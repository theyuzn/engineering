#!/usr/bin/env python3

import argparse
import random
from turtle import Screen, Turtle


class Player:
    def __init__(self, index, num_of_players):
        self.index = index
        self.num_of_players = num_of_players

    def put(self, floors):
        f = random.choice(floors)
        a, b = random.sample(range(self.num_of_players), 2)
        return f, a, b


class Game:
    def __init__(self, players, num_of_ladders):
        self.players = players
        self.num_of_ladders = num_of_ladders
        self.num_of_total_ladders = len(players) * num_of_ladders
        self.board = [[] for i in range(len(players))]
        for i in range(len(players)):
            self.board[i] = [i] * self.num_of_total_ladders

    def play(self):
        floors = list(range(self.num_of_total_ladders))
        for i in range(self.num_of_ladders):
            for p in self.players:
                f, a, b = p.put(floors)
                floors.remove(f)
                self.board[a][f], self.board[b][f] = self.board[b][f], self.board[a][f]


class Animation:
    def __init__(self, board):
        self.board = board
        self.screen = Screen()

    def animate(self):
        num_of_players = len(self.board)
        num_of_ladders = len(self.board[0])

        xstep = 72
        ystep = 24

        width = xstep * num_of_players
        height = ystep * num_of_ladders

        self.screen.bgcolor("black")

        self.screen.setup(width, height)
        self.screen.setworldcoordinates(-xstep, -ystep, xstep + width, ystep + height)

        turtles = [Turtle(shape="circle", visible=False) for i in range(num_of_players)]

        for i, t in random.sample(list(enumerate(turtles)), len(turtles)):
            t.color("white")
            t.speed(0)
            t.setheading(90)
            t.penup()
            t.goto(xstep * i, 0)
            t.pendown()
            t.forward(ystep * num_of_ladders)

        for i, t in random.sample(list(enumerate(turtles)), len(turtles)):
            t.speed(10)
            t.penup()
            t.goto(xstep * i, 0)
            t.setheading(0)
            for j in range(num_of_ladders):
                if self.board[i][j] > i:
                    t.penup()
                    t.goto(xstep * i, ystep * j)
                    t.pendown()
                    t.forward(xstep * (self.board[i][j] - i))

        for i, t in enumerate(turtles):
            color = (random.random(), random.random(), random.random())
            t.penup()
            t.color(color)
            t.setheading(90)
            t.goto(xstep * i, 0)
            t.showturtle()

            cursor = Turtle(shape="circle", visible=False)
            cursor.color(color)
            cursor.speed(8)
            cursor.setheading(90)
            cursor.penup()
            cursor.goto(xstep * i, 0)
            cursor.pendown()
            cursor.showturtle()

            line = i
            for j in range(num_of_ladders):
                if self.board[line][j] > line:  # move east
                    cursor.setheading(0)
                    cursor.forward(xstep * (self.board[line][j] - line))
                    cursor.setheading(90)
                    cursor.forward(ystep)
                    line = self.board[line][j]
                elif self.board[line][j] < line:  # move west
                    cursor.setheading(90)
                    cursor.forward(-3)
                    cursor.setheading(180)
                    cursor.forward(xstep * (line - self.board[line][j]))
                    cursor.setheading(90)
                    cursor.forward(3)
                    cursor.forward(ystep)
                    line = self.board[line][j]
                else:  # move north
                    cursor.setheading(90)
                    cursor.forward(ystep)

    def clear(self):
        self.screen.clearscreen()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="stair climbing")

    parser.add_argument(
        "-n", dest="num_of_players", type=int, help="num of players", default=10
    )
    parser.add_argument(
        "-m",
        dest="num_of_ladders",
        type=int,
        help="num of ladders each player can put",
        default=3,
    )
    parser.add_argument("-c", dest="count", type=int, help="repeat count", default=1)

    opts = parser.parse_args()

    for i in range(opts.count):
        players = [Player(i, opts.num_of_players) for i in range(opts.num_of_players)]

        game = Game(players, opts.num_of_ladders)
        game.play()

        animation = Animation(game.board)
        animation.animate()
        animation.clear()