import Data.List (permutations, transpose, unfoldr)
import qualified Data.Set as Set

------------- Chunks_k -------------

chunksOf :: Int -> [a] -> [[a]] -- (from Data.List.Split)
chunksOf n = takeWhile (not . null) . unfoldr (Just . splitAt n)

chunksKDisorder :: Int -> String -> Int
chunksKDisorder k chain = length . Set.fromList $ chunksOf (2 ^ k) chain

------------- Concat Times -------------

ifEqualHalves :: (String -> Int) -> (String, String) -> Int
ifEqualHalves function (a, b)
  | a == b = function a
  | otherwise = function a + function b

ctDisorder :: String -> Int
ctDisorder [_] = 1
ctDisorder chain = ifEqualHalves ctDisorder pairOfHalves
  where
    pairOfHalves = splitAt (length chain `div` 2) chain

ctDisorder' :: String -> Int
ctDisorder' [_] = 1
ctDisorder' chain = ifEqualHalves ctDisorder' pairOfHalves
  where
    pairOfHalves = splitAtDimension (2 ^ 0) chain

------------- Cube -------------

splitAtDimension :: Int -> String -> (String, String)
splitAtDimension d chain = (zeros, ones)
  where
    m = length chain `div` (d + 1)
    zeros = filterIndex (\i -> i `mod` 2 * m < m) chain
    ones = filterIndex (\i -> i `mod` 2 * m >= m) chain
    filterIndex p xs = [x | (x, i) <- zip xs [0 ..], p i]

cubeDisorder :: String -> Int
cubeDisorder [_] = 1
cubeDisorder chain =
  minimum $ map (ifEqualHalves cubeDisorder) pairsOfHalves
  where
    pairsOfHalves =
      map
        (`splitAtDimension` chain)
        [1 .. length chain `div` 2]

------------- All equivalent chains -------------

-- [[0,0,0], [0,0,1]... for log2(n) = 3
truthTableInputs :: Int -> [[Int]]
truthTableInputs 1 = [[]]
truthTableInputs n = do
  b <- [0, 1]
  map (b :) (truthTableInputs (n `div` 2))

-- returns all valid permutations for an input exchange (each of size n)
allBFPermutations :: Int -> [[Int]]
allBFPermutations n = map fromBFtoPerm basicFunctions
  where
    basicFunctions = permutations . transpose . truthTableInputs $ n
    fromBFtoPerm bf = map binaryToDecimal $ transpose bf
    binaryToDecimal = foldl (\acc x -> x + 2 * acc) 0

-- returns all equivalent chains
chainPerms :: String -> [String]
chainPerms chain = map applyPermutation $ allBFPermutations (length chain)
  where
    applyPermutation perm = map (chain !!) perm

-- given metric and chain, returns min disorder for all equivalent chains
minEquivDisorder :: (Ord c) => (String -> c) -> String -> c
minEquivDisorder metric = minimum . map metric . chainPerms
