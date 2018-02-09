import Data.List

func [] _ = ""
func _ [] = ""
func (a:b) (c:d)
     | a==c = (a:(func b d))
     | otherwise = ""

main = do
      str1 <- getLine
      str2 <- getLine
      let s1 = func str1 str2
          s2 = str1\\s1
          s3 = str2\\s1
      putStrLn ((show (length s1)) ++ " " ++ s1)
      putStrLn ((show (length s2)) ++ " " ++ s2)
      putStrLn ((show (length s3)) ++ " " ++ s3)