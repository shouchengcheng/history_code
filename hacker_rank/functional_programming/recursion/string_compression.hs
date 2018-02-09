func n (a:b)
 | (b == [])&&(n/=1) = (a:show n)
 | (b == [])&&(n==1) = (a:"")
 | a == head b = func (n+1) b
 | (a /= head b)&&(n /= 1) = (a:(show n)) ++ (func 1 b)
 | (a /= head b)&&(n == 1) = (a:(func 1 b))
 | otherwise = ""

main = do
      str <- getLine
      putStrLn $ func 1 str
	  
