module FileSystem (
    getRecursiveContents,
    FileInfoP,
    liftP,
    sizeP,
    pathP,
    liftPath,
    equalP, (==?),
    greaterP, (>?),
    lesserP, (<?),
    andP, (&&?),
    orP, (||?),
    atMostThreePictures,
    find,
    hind
) where

import Control.Monad (forM, filterM, liftM)
import Data.Char (toLower)
import System.Directory (
    Permissions(..),
    doesDirectoryExist,
    getModificationTime,
    listDirectory,
    getPermissions
    )
import System.FilePath ((</>), takeExtension, takeFileName)
import Data.Time (UTCTime)
import Control.Exception (handle, bracket, SomeException)
import System.IO (IOMode(..), hClose, hFileSize, openFile)

{- type Predicate = FilePath
            -> Permissions
            -> Maybe Integer
            -> UTCTime
            -> Bool -}

type FileInfoP a = FilePath
            -> Permissions
            -> Maybe Integer
            -> UTCTime
            -> a

data FileInfo = FileInfo {
      infoPath :: FilePath
    , infoPerms :: Maybe Permissions
    , infoSize :: Maybe Integer
    , infoModTime :: Maybe UTCTime
} deriving (Eq, Ord, Show)

data Iterate seed = Done { unwrap :: seed }
                  | Skip { unwrap :: seed }
                  | Continue { unwrap :: seed }
                  deriving (Show)
type Iterator seed = seed -> FileInfo -> Iterate seed

atMostThreePictures :: Iterator [FilePath]
atMostThreePictures paths info
        | length paths == 3
            = Done paths
        | isDirectory info && takeFileName path == ".svn"
            = Skip paths
        | extension `elem` [".jpg", ".png"]
            = Continue (path : paths)
        | otherwise
            = Continue paths
    where extension = map toLower (takeExtension path)
          path = infoPath info

getInfo :: FilePath -> IO FileInfo
getInfo path = do
    perms <- maybeIO $ getPermissions path
    size <- maybeIO (bracket (openFile path ReadMode) hClose hFileSize)
    modified <- maybeIO (getModificationTime path)
    return $ FileInfo path perms size modified

maybeIO :: IO a -> IO (Maybe a)
maybeIO act = handle handler (Just `liftM` act)
            where handler :: SomeException -> IO (Maybe a)
                  handler = \_ -> return Nothing

liftPath :: (FilePath -> a) -> FileInfoP a
liftPath f w _ _ _ = f w
pathP :: FileInfoP FilePath
pathP path _ _ _ = path

sizeP :: FileInfoP Integer
sizeP _ _ (Just size) _ = size
sizeP _ _ Nothing _ = -1

isDirectory :: FileInfo -> Bool
isDirectory = maybe False searchable . infoPerms

equalP, (==?) :: (Eq a) => FileInfoP a -> a -> FileInfoP Bool
equalP = liftP (==)
(==?) = equalP
infix 4 ==?

greaterP, lesserP, (>?), (<?) :: (Ord a) => FileInfoP a -> a -> FileInfoP Bool
greaterP = liftP (>)
(>?) = greaterP
lesserP = liftP (<)
(<?) = lesserP
infix 4 >?, <?

andP, orP, (&&?), (||?) :: FileInfoP Bool -> FileInfoP Bool -> FileInfoP Bool
andP = liftP2 (&&)
(&&?) = andP
orP = liftP2 (||)
(||?) = orP
infixr 3 &&?, ||?

constP :: a -> FileInfoP a
constP k _ _ _ _ = k

liftP :: (a -> b -> c) -> FileInfoP a -> b -> FileInfoP c
-- liftP op f k w x y z = f w x y z `op` k
liftP op f k = liftP2 op f (constP k)

liftP2 :: (a-> b -> c) -> FileInfoP a -> FileInfoP b -> FileInfoP c
liftP2 op f g w x y z= f w x y z `op` g w x y z

find :: ([FileInfo] -> [FileInfo]) -> FilePath -> IO [FileInfo]
find order path = do
    names <- listDirectory path
    contents <- mapM getInfo (path : map (path </>) names)
    liftM concat $ forM (order contents) $ \info -> do
        if isDirectory info && infoPath info /= path
            then find order $ infoPath info
            else return [info]

{- hind :: Predicate -> FilePath -> IO [FilePath]
hind pred path = getRecursiveContents path >>= filterM check
    where check name = do
            perms <- getPermissions name
            size <- getFileSize name
            modified <- getModificationTime name
            return $ pred name perms size modified -}

hind :: Iterator a -> a -> FilePath -> IO a
hind iter initSeed path = do
    endSeed <- fold initSeed path
    return (unwrap endSeed)
  where
    fold seed subpath = listDirectory subpath >>= walk seed
    walk seed (name:names) = do
        let path' = path </> name
        info <- getInfo path'
        case iter seed info of
            done@(Done _) -> return done
            Skip seed' -> walk seed' names
            Continue seed'
                | isDirectory info -> do
                    next <- fold seed' path'
                    case next of
                        done@(Done _) -> return done
                        seed'' -> walk (unwrap seed'') names
                | otherwise -> walk seed' names
    walk seed _ = return (Continue seed)

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

{- simpleFind :: (FilePath -> Bool) -> FilePath -> IO [FilePath]
simpleFind pred path = do
    names <- getRecursiveContents path
    return (filter pred names) -}