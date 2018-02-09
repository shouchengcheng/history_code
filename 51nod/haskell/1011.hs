gdc:: Int->Int->Int
gdc n m 
    | n==m = n
    | n>m = gdc (n-m) m
    | otherwise = gdc n (m-n)

main = do
     input <- getLine
     let s = map read $ words input
     putStrLn . show $ gdc (head s) (last s)