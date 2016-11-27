## DS1620 demo project

In this project i demonstrate working with DS1620 temperature sensor on STM32F4Discovery dev. board.

Connection diagram:
```
|--------|----------|------------------|
| DS1620 |          | F4Discovery GPIO |
|--------|----------|------------------|
| DQ     | <======> | B12              |
| CLK    | <======> | B13              |
| RST    | <======> | B11              |
|--------|----------|------------------|
```