

# Dev Log

## Day 1
Initialized project.
Decided to use []Point for snake body.

# notes:
- go mod init github.com/yourname/snake
~ This project’s import path is github.com/yourname/snake”

if go mod init snake
~ This works locally, but:
* not great for publishing
* can cause confusion later
* not standard practice

## Day 2
Implemented movement.
Need to prevent instant reverse direction.