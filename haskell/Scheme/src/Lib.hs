module Lib
    ( readExpr
    ) where

import Text.ParserCombinators.Parsec hiding (spaces)

someFunc :: IO ()
someFunc = putStrLn "someFunc"

symbol :: Parser Char
symbol = oneOf "!#$%&|*+-/:<=>?@^_~"

readExpr :: String -> String
readExpr input = case parse symbol "lisp" input of
    Left err -> "No match: " ++ show err
    Right val -> "Found value"
