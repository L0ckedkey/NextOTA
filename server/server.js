const express = require('express');
const path = require('path');
const WebSocket = require('ws');
const multer = require('multer');
const fs = require('fs');

const upload = multer({ dest: 'uploads/' });

const app = express();
const port = 3000;
app.use(express.json());

class Controller {
    constructor(ip, mac, name) {
        this.ip = ip;
        this.mac = mac;
        this.name = name;
        this.lastSeen = Date.now();
    }
}

const TIMEOUT_THRESHOLD = 2000;

function cleanUpOldDevices() {
    const now = Date.now();
    for (let i = controllers.length - 1; i >= 0; i--) {
        if (now - controllers[i].lastSeen > TIMEOUT_THRESHOLD) {
            console.log('Removing inactive controller:', controllers[i]);
            controllers.splice(i, 1);
            broadcastControllers();
        }
    }
}

setInterval(cleanUpOldDevices, 1000);

const controllers = [];

const server = app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});

const wss = new WebSocket.Server({ server });

wss.on('connection', ws => {
    console.log('Client connected');
    ws.on('close', () => console.log('Client disconnected'));
});

function broadcastControllers() {
    const data = JSON.stringify(controllers);
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(data);
        }
    });
}

app.use(express.static(path.join(__dirname, 'public')));

app.post('/register', (req, res) => {
    console.log(req.body)
    const { ip, mac, name } = req.body;

    const existingController = controllers.find(controller => controller.mac === mac);

    if (existingController) {
        console.log('Controller with MAC already exists:', existingController);
        res.status(200).json({ message: 'Controller already registered', controller: existingController });
    } else {
        const newController = new Controller(ip, mac, name);
        controllers.push(newController);
        console.log('Controller added:', newController);
        broadcastControllers();

        res.status(201).json({ message: 'Controller added successfully', controller: newController });
    }
});


app.post('/upload', upload.single('file'), (req, res) => {
    if (!req.file) {
        return res.status(400).json({ message: 'No file uploaded' });
    }

    console.log('File uploaded:', req.file.filename);
    res.status(200).json({ message: 'File uploaded successfully', filename: req.file.filename });
});

app.post('/distribute', (req, res) => {
    const { filename } = req.body;
    const filePath = path.join(__dirname, 'uploads', filename);

    if (!fs.existsSync(filePath)) {
        return res.status(404).json({ message: 'File not found' });
    }

    controllers.forEach(controller => {
        const url = `http://${controller.ip}:3000/update`; // Endpoint for boards to get the firmware
        // Send HTTP request to each board to start the update
        fetch(url, {
            method: 'POST',
            body: JSON.stringify({ file: filename }),
            headers: { 'Content-Type': 'application/json' }
        })
        .then(response => response.json())
        .then(data => console.log(`Update sent to ${controller.ip}:`, data))
        .catch(error => console.error(`Failed to update ${controller.ip}:`, error));
    });

    res.status(200).json({ message: 'Firmware distribution initiated' });
});

app.post('/alive', (req, res) => {
    const { ip, mac, name } = req.body;
    const controller = controllers.find(ctrl => ctrl.mac === mac);
    if (controller) {
        controller.lastSeen = Date.now();
        res.status(200).json({ message: 'Alive status updated' });
    } else {
        const newController = new Controller(ip, mac, name);
        controllers.push(newController);
        console.log('Controller added:', newController);
        broadcastControllers();
    }
});



app.get('/devices', (req, res) => {
    res.json(controllers);
});
