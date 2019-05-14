# High level data overview


## LoRaWAN messages
### Data messages
- Node
- Boot counter
- **Timestamp**
- Repeating blocks of:
    - Sensor ID
    - **Data values**

One `data message` becomes many rows in the `data table`.

### Configuration messages
- Node
- Boot counter
- GPS position
- Repeating blocks of:
    - Sensor ID
    - **Data labels**
    - **Units**

One `configuration message` becomes one row in the `node configuration table` and many rows in the `data configuration table`.

## Database Schema

### Data table
- Node
- Boot counter
- Sensor ID
- Parameter
- **Read time**
- Store time
- **Reading**

### Node configuration table
- Node
- Boot counter
- **GPS position**
- **Description**

### Parameter configuration table
- _Node_
- _Boot counter_
- Sensor ID
- Parameter
- **Data label**
- **Unit**

### User information table
- Node
- Time
- Description


The node and sensor configuration tables are relational. One node configuration maps to many sensor configurations.

## Refference
Data field descirption:
- Node (id of node sending message)
- Boot counter (count of how many power-cycles node has done. New configuration can only be applied after power-cycle.)
- Sensor ID (determined by which terminal block on the node the senor is connected to, see silkscreen labels)
- Parameter (inedx oultining which sensor value is encoded)
- Read time (date and timestamp for when data was collected)
- Reading (data value, the point of the exersise)
