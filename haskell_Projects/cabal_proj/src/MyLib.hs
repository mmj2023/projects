module MyLib (someFunc) where
import System.Directory
someFunc :: IO ()
someFunc = do
  contentts <- listDirectory "src"
  putStrLn (show contentts)
