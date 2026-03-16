# miniredis
A lightweight in-memory key-value store built in C++, inspired by Redis.

## Features
- SET / GET / DEL / EXISTS commands
- TTL/expiry support
- Persistence to file
- Multi-client support via threading

## Build
cmake -B build
cmake --build build

## Usage
Run the server:
./build/server

Run the client:
./build/client

## Commands
| Command | Example | Description |
|---------|---------|-------------|
| SET | SET name John | Store a value |
| SET EX | SET session abc EX 10 | Store with TTL in seconds |
| GET | GET name | Retrieve a value |
| DEL | DEL name | Delete a key |
| EXISTS | EXISTS name | Check if key exists |
| PING | PING | Health check |
| EXIT | EXIT | Disconnect |