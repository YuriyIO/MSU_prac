module Show
    (
        window,
        drawGame
    ) where

import Graphics.Gloss
import Graphics.Gloss.Interface.Pure.Game()

import Const
import Rules
 
type Shape = (Float -> Float -> Picture)

table :: (Int, Int)
table = (leftDown, upRight)

cellMap :: (a -> b) -> (a, a) -> (b, b)
cellMap f (a, b) = (f a, f b)

toInt :: Float -> Int
toInt num = floor num

toFloat :: Int -> Float
toFloat a = fromIntegral a

cellToPoint :: Cell -> Point
cellToPoint cll = cellMap toFloat cll

lft :: [Cell]
lft = [(leftDown, x)| x <- [leftDown..upRight]]

dwn :: [Cell]
dwn = [(x, leftDown)| x <- [leftDown..upRight]]

up :: [Cell]
up = [(x, upRight)| x <- [leftDown..upRight]]

rght :: [Cell]
rght = [(upRight, x)| x <- [leftDown..upRight]]

walls :: [Cell]
walls = lft ++ dwn ++ up ++ rght

screenSize :: Int
screenSize = ((snd table) - (fst table) + 1) * (toInt size)

window :: Display
window = InWindow "Snake" (screenSize, screenSize) (0, 0)


drawGame :: Field -> Picture
drawGame fld = if (gameOver fld)
                    then drawGameOver fld
                else drawField fld


drawGameOver :: Field -> Picture
drawGameOver fld = pictures [drawGO, drawResults fld, drawBestRes fld]

drawGO :: Picture
drawGO = translate (-200) (200) $ color blue $ scale 0.5 0.5 $
         text "GAME OVER"

drawResults :: Field -> Picture
drawResults fld = translate (-200) (100) $ color blue $ scale 0.2 0.2 $
                  text ("Results: " ++ (convToString (results fld)))
 
convToString :: [Int] -> String
convToString [] = ""
convToString (x:xs) = show x ++ " " ++ (convToString xs)

drawBestRes :: Field -> Picture
drawBestRes fld = translate (-200) (0) $ color blue $ scale 0.2 0.2 $ 
                  text ("The best result: " ++ show (maximum (results fld)))


drawField :: Field -> Picture
drawField fld = pictures [drawSnake (snake fld), drawApple (apple fld),
                          drawWalls, drawLevel fld, drawResult fld]

drawLevel :: Field -> Picture
drawLevel fld = translate (200) (270) $ color blue $ scale 0.2 0.2 $ 
                text ("Level: " ++ show(level fld))

drawResult :: Field -> Picture
drawResult fld = translate (189) (240) $ color blue $ scale 0.2 0.2 $ 
                 text ("Result: " ++ show(result fld))

drawApple :: Cell -> Picture
drawApple pnt = drawCell appleCollor rectangleSolid (toPoint pnt)

drawSnake :: [Cell] -> Picture
drawSnake sn = drawArr (map toPoint sn) snakeCollor rectangleWire

drawWalls :: Picture
drawWalls = drawArr (map toPoint walls) edgeCollor rectangleSolid

drawArr :: [Point] -> Color -> Shape -> Picture
drawArr arr clr shp = pictures (map (drawCell clr shp) arr)

drawCell :: Color -> Shape -> Point -> Picture
drawCell col shape pnt = translate (fst pnt) (snd pnt) $ color col $ shape size size

toPoint :: Cell -> Point
toPoint cll = cellMap ((+ size/2) . (* size)) (cellToPoint cll)
