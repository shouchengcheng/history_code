gcd'::Int->Int->Int
gcd' a b
    | a>b = gcd' (a-b) b
    | a<b = gcd' a (b-a)
    | otherwise = a

fun::Int->Int->Int
fun a b =
    if label > 1
       then label*(fun (div a label) (div b label))
       else a*b
    where label = gcd' a b

main = do
     input <- getLine
     let [a, b] = map (read::String->Int) $ words input
     putStrLn . show $ fun a b 