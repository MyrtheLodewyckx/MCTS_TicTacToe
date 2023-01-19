# TicTacToe_MCTS
A simple game of TicTacToe where your opponent uses a Monte Carlo tree search algorithm.


## Monte Carlo Tree Search?
Unlike breath first search or depth first search, this tree is being made while searching.
It does so in 4 steps:
- Selection
- Expantion
- Simulation
- Back Propagation

Starting from a certain state this algorithm will find the best next step by simulating it's options and weighing the value of each outcome.

### Selection
This is where you choose which node you'd want to explore, by looking at which one is most promising. This is not done bu only looking at which one has had the most 'wins' in this case, but also by looking at which ones havent been visited as much yet. There might be some hidden potential there. Therefore I used the UCT formula. This gives me a value I can actually compare.

### Expansion
After choosing a node, I will expand it by making a child node with a possible action. 

### Simulation
Now that the tree has been expanded, I will simulate a game starting from the freshly made node. The outcome of said game will give me a value.

### Back Propagation
Finally, I have a value we can work with. I will backtrack, updationg every node with the new rersult.

### Iterations
This process will loop for as many times as you think necessairy. More iterations mean a better result, however keep in mind that a lot of iteration can take a lot of time. Use this to your advantage. You don't want the AI to be unbeatable right? Since a lot of this is also done by chance, you can simulate mistakes us humans can make as well.

![ezgif com-gif-maker](https://user-images.githubusercontent.com/99867927/213260532-482cef9a-bc9f-49d2-b6cc-2dde7863130e.gif)

## Implementation

For this I seperated the game and the tree so I can use the algorithm for other games as well.
Aside from this I made three classes necessairy for the game.

![GameClasses](https://user-images.githubusercontent.com/99867927/213276879-a27dfb8c-5b88-4449-b93c-ada2c8c91c4a.png)
