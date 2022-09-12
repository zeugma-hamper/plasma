#!/usr/bin/runhaskell

processLine :: String -> String
processLine s =
  "  " ++ show (s ++ "\n")

processFile :: String -> String
processFile s =
  let l = lines s
      changed = map processLine l
  in unlines changed

main = interact processFile
