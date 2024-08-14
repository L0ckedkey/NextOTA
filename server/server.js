const express = require('express');
const path = require('path');
const app = express();
const port = 3000;
app.use(express.json());

class Controller {
    constructor(ip, mac, name) {
        this.ip = ip;
        this.mac = mac;
        this.name = name;
    }
}

const controllers = [];
const devices = [
    { ip: '192.168.10.1', type: 'ESP8266' },
    { ip: '192.168.10.2', type: 'ESP32' }
];

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.post('/api/controller', (req, res) => {
    const { ip, mac, name } = req.body;
    const newController = new Controller(ip, mac, name);
    controllers.push(newController);
    console.log('Controller added:', newController);
    res.status(201).json({ message: 'Controller added successfully', controller: newController });
});

app.get('/api/devices', (req, res) => {
    res.json(controllers);
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
