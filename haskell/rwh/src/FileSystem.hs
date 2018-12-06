module FileSystem (
    getRecursiveContents,
    FileInfoP,
    liftP,
    sizeP,
    pathP,
    equalP,
    greaterP,
    lesserP,
    hind
) where

import Control.Monad (forM, filterM)
import System.Directory (
    Permissions(..),
    doesDirectoryExist,
    getModificationTime,
    listDirectory,
    getPermissions
    )
import System.FilePath ((</>), takeExtension)
import Data.Time (UTCTime)
import Control.Exception (handle, bracket, SomeException)
import System.IO (IOMode(..), hClose, hFileSize, openFile)

type Predicate = FilePath
            -> Permissions
            -> Maybe Integer
            -> UTCTime
            -> Bool

type FileInfoP a = FilePath
            -> Permissions
            -> Maybe Integer
            -> UTCTime
            -> a
    
pathP :: FileInfoP FilePath
pathP path _ _ _ = path

sizeP :: FileInfoP Integer
sizeP _ _ (Just size) _ = size
sizeP _ _ Nothing _ = -1

liftP :: (a -> b -> c) -> FileInfoP a -> b -> FileInfoP c
liftP op f k w x y z = f w x y z `op` k

equalP :: (Eq a) => FileInfoP a -> a -> FileInfoP Bool
equalP = liftP (==)

greaterP, lesserP :: (Ord a) => FileInfoP a -> a -> FileInfoP Bool
greaterP = liftP (>)
lesserP = liftP (>)

hind :: Predicate -> FilePath -> IO [FilePath]
hind pred path = getRecursiveContents path >>= filterM check
    where check name = do
            perms <- getPermissions name
            size <- getFileSize name
            modified <- getModificationTime name
            return $ pred name perms size modified

getRecursiveContents :: FilePath -> IO [FilePath]
getRecursiveContents topDir = do
    names <- listDirectory topDir
    paths <- forM names $ \name -> do
        let path = topDir </> name
        isDirectory <- doesDirectoryExist path
        if isDirectory
            then getRecursiveContents path
            else return [path]
    return $ concat paths

getFileSize :: FilePath -> IO (Maybe Integer)
getFileSize path = handle handler $
            bracket (openFile path ReadMode) hClose $ \h -> do
                        size <- hFileSize h
                        return $ Just size
            where handler :: SomeException -> IO (Maybe Integer)
                  handler = \_ -> return Nothing

simpleFind :: (FilePath -> Bool) -> FilePath -> IO [FilePath]
simpleFind pred path = do
    names <- getRecursiveContents path
    return (filter pred names)