module Rules
    (  
        Field(..),
        Cell,
        Snake,
        snakeInit,
        prossGame,
        event,
        steps
    ) where


import System.Random
import Graphics.Gloss()
import Graphics.Gloss.Interface.Pure.Game

import Const

type Cell = (Int, Int) 

type Snake = [Cell]

data Direction = UP | DOWN | LEFT | RIGHT  deriving (Eq, Show)  
 
data Field = Field 
    {
        
        snake :: Snake,
        apple :: Cell,
        gen :: StdGen,
        direction :: Direction,
        result :: Int,
        results :: [Int],
        level :: Int,
        gameOver :: Bool
    } deriving (Show, Eq)



snakeInit :: StdGen -> Field
snakeInit gn = Field
    {
        snake = [(-6, 9), (-7, 9), (-8, 9), (-9, 9)],
        apple = (1, 4),
        gen = gn,
        direction = RIGHT,
        result = 0,
        results = [],
        level = 1,
        gameOver = False
    }


getNewApple :: StdGen -> Cell
getNewApple gn = convToCell(getRandomPoss gn)

getNewRandomGen :: StdGen -> StdGen
getNewRandomGen gn = snd (randomR ((leftDown+1), (upRight-1)) gn)

getRandomPoss :: StdGen -> [Int]
getRandomPoss gn = take 2 $ randomRs ((leftDown+1), (upRight-1)) gn

convToCell :: [Int] -> Cell
convToCell l = (l!!0, l!!1)

eqCell:: Cell -> Cell -> Bool
eqCell a b = fst a == fst b && snd a == snd b

elemCell:: Cell -> [Cell] -> Bool
elemCell _ [] = False
elemCell c (x:xs) = eqCell c x || elemCell c xs


nextCell :: Snake -> Direction -> Cell
nextCell sn RIGHT = (fst (sn!!0) + 1, snd (sn!!0))
nextCell sn LEFT = (fst (sn!!0) - 1, snd (sn!!0))
nextCell sn UP = (fst (sn!!0), snd (sn!!0) + 1)
nextCell sn DOWN = (fst (sn!!0), snd (sn!!0) - 1)


shiftSnake :: Snake -> Cell -> Snake
shiftSnake sn cll = take (length sn) (cll : sn)

bumpIntoWalls :: Cell -> Bool
bumpIntoWalls step = fst step == leftDown || fst step == upRight
                  || snd step == leftDown || snd step == upRight

checkGameOver :: Snake -> Cell -> Bool
checkGameOver sn step = (bumpIntoWalls step) || (elemCell step sn) 

doNothing :: Field -> Cell-> Bool
doNothing fld step = ((level fld == 1) && (checkGameOver (snake fld) step)) 
                  || ((level fld == 2) && (bumpIntoWalls step))
                    

makeStep::Field -> Cell -> Field
makeStep fld cll =      if(gameOver fld)
                            then fld
                        else if (eqCell (apple fld) cll)
                            then fld
                                    {
                                        snake = cll : snake fld,
                                        apple = getNewApple (gen fld),
                                        gen = getNewRandomGen (gen fld),
                                        result = (result fld)+1
                                    }
                        else if(doNothing fld cll)
                                then fld
                        else if (checkGameOver (snake fld) cll) 
                            then  fld
                                    {
                                        results = (result fld):(results fld),
                                        gameOver = True
                                    }
                        else fld
                                {
                                    snake = shiftSnake (snake fld) cll
                                }
    
prossGame :: Float -> Field  -> Field
prossGame _ fld =  makeStep fld (nextCell (snake fld) (direction fld))     


resetGame :: Field -> Field
resetGame game = game
                    {
                        snake = [(-6, 9), (-7, 9), (-8, 9), (-9, 9)],
                        apple = (1, 4),
                        direction = RIGHT,
                        result = 0,
                        gameOver = False
                    }


event :: Event -> Field -> Field
event (EventKey (Char 'w') Down _ _) fld = fld{direction = UP}
event (EventKey (Char 'a') Down _ _) fld = fld{direction = LEFT}
event (EventKey (Char 's') Down _ _) fld = fld{direction = DOWN}
event (EventKey (Char 'd') Down _ _) fld = fld{direction = RIGHT}
event (EventKey (Char 'r') Down _ _) fld =  resetGame fld
event (EventKey (Char '1') Down _ _) fld =  fld{level = 1}--can't die
event (EventKey (Char '2') Down _ _) fld =  fld{level = 2}--hit into walls
event (EventKey (Char '3') Down _ _) fld =  fld{level = 3}--walls and snake
event _ s = s


steps :: Int
steps = 5
