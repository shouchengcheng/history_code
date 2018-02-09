import Data.List

func' [] = ""
func' (a:b) 
		| b == [] = (a:"")
		| a == head b = func' b
		| otherwise = (a:func' b)

func [] = ""
func (a:b)
       | b == [] = (a:"")
       | otherwise = (a:(func (delete' a b)))

delete' a [] = ""	   
delete' a (b:c)
	| a == b = delete' a c
	| otherwise = (b:(delete' a c))
	
	   
main = do
     str1 <- getLine
     putStrLn $ (func $ func' str1)