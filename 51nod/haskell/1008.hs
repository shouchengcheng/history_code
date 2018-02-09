
fun 0 = 0
fun 1 = 1
fun n = n*(fun (n-1))

main = do
     input <- getLine
     let s = map read $ words input
     putStrLn . show $ mod (fun $ head s) (last s)