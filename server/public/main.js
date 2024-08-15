class Controller {
    constructor(ip, mac, name) {
        this.ip = ip;
        this.mac = mac;
        this.name = name;
    }
}

document.addEventListener('DOMContentLoaded', function () {
    // WebSocket connection
    const ws = new WebSocket(`ws://${window.location.host}`);

    ws.onmessage = function (event) {
        const devices = JSON.parse(event.data);
        const deviceListContainer = document.querySelector('.device-list');
        deviceListContainer.innerHTML = ''; // Bersihkan daftar sebelum memperbarui

        devices.forEach(device => {
            const deviceDiv = document.createElement('div');
            deviceDiv.style.color = '#ECFFE6';
            deviceDiv.style.fontSize = '18px';
            deviceDiv.style.wordWrap = 'break-word';
            deviceDiv.className = 'roboto-light';
            deviceDiv.textContent = `${device.ip} - ${device.name}`;

            deviceListContainer.appendChild(deviceDiv);
        });
    };

    // Initial fetch to load devices on page load
    fetch('/devices')
        .then(response => response.json())
        .then(devices => {
            const deviceListContainer = document.querySelector('.device-list');
            devices.forEach(device => {
                const deviceDiv = document.createElement('div');
                deviceDiv.style.color = '#ECFFE6';
                deviceDiv.style.fontSize = '18px';
                deviceDiv.style.wordWrap = 'break-word';
                deviceDiv.className = 'roboto-light';
                deviceDiv.textContent = `${device.ip} - ${device.name}`;

                deviceListContainer.appendChild(deviceDiv);
            });
        })
        .catch(error => console.error('Error fetching devices:', error));
});

document.getElementById('upload-file').onclick = function() {
    document.getElementById('fileInput').click();
};

document.getElementById('fileInput').onchange = function(event) {
    const file = event.target.files[0];
    console.log(file);
    document.getElementById('file-name').textContent = `${file.name}`;
};
