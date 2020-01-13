# Gomoku-NeuralNetwork | WIP
C program for training multiple Neural Network

build: `make`  
launch: `./Gomoku`


## Available commands:

- `set [variable] [value]` | variable: any variable name   |   value: value to set

- `set nn[spec] [variable] [value]` | spec: spec number | variable: any variable name   |   value: value to set
    
- `new player [type]` | type: nn, nn[spec], ia or human
      
- `new pool`

- `new game [id] [id]` | id : player id 

- `new spec [(type])` | type: optional, type of nn (0 or 1)

- `rm player [id]` | id : player id

- `rm pool`

- `rm spec [spec]` | spec : spec number

- `train start ([nb_gen])` | nb_gen : optional, -1 for illimited

- `train stop`

- `load player [file]` | file : file to load

- `load pool`

- `save player [id]` | id : player id 

- `save pool`

- `list player [id]` | id : player id

- `list players`

- `refresh`

- `config`

- `config nn`
