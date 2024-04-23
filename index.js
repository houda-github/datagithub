
// serverhttp
const http = require('http');
const url = require('url');
const fs = require('fs');
const path = require('path');
const mongoose = require('mongoose');

// MongoDB connection URL
const mongoURL = 'mongodb://localhost:27017/database';

// Connect to MongoDB using Mongoose
mongoose.connect(mongoURL, { useNewUrlParser: true, useUnifiedTopology: true });
const db = mongoose.connection;

// Define a schema for the data
const dataSchema = new mongoose.Schema({
  temperature: Number,
  humidity: Number,
  force: Number
});

// Define a model based on the schema
const DataModel = mongoose.model('Data', dataSchema);

// Create a server HTTP
const server = http.createServer((req, res) => {
  // Analyze the request URL
  const parsedUrl = url.parse(req.url, true);

  // Check the URL path
  if (parsedUrl.pathname === '/update') {
    // Check if the HTTP method is GET
    if (req.method === 'GET') {
      // Retrieve temperature, humidity, and force data from URL parameters
      const temp = parseFloat(parsedUrl.query.temp);
      const humidity = parseFloat(parsedUrl.query.humidity);
      const force = parseInt(parsedUrl.query.force);

      // Display received data in the server console
      console.log('Temperature:', temp, '°C');
      console.log('Humidity:', humidity, '%');
      console.log('Force:', force);

      // Create a new document instance
      const newData = new DataModel({ temperature: temp, humidity: humidity, force: force });

      // Save the data to MongoDB
      newData.save()
        .then(() => {
          console.log('Data saved to MongoDB successfully');
          // Send an HTTP response to the client
          res.writeHead(200, { 'Content-Type': 'text/plain' });
          res.end('Data received and saved successfully');
        })
        .catch(err => {
          console.error('Failed to save data to MongoDB:', err);
          res.writeHead(500, { 'Content-Type': 'text/plain' });
          res.end('Internal Server Error');
        });
    } else {
      // If the HTTP method is not GET, return an error
      res.writeHead(405, { 'Content-Type': 'text/plain' });
      res.end('Method Not Allowed');
    }
  } else if (parsedUrl.pathname === '/sensorData') {
    // Serve the latest sensor data as JSON
    DataModel.findOne().sort({$natural:-1}).exec() // Utilisation de la promesse
      .then(data => {
        if (!data) {
          res.writeHead(404, { 'Content-Type': 'text/plain' });
          res.end('Sensor data not found');
        } else {
          // Send the sensor data as JSON response
          res.writeHead(200, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify(data));
        }
      })
      .catch(err => {
        console.error('Failed to retrieve sensor data from MongoDB:', err);
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Internal Server Error');
      });
  } else if (parsedUrl.pathname === '/') {
    // Serve the HTML page
    const filePath = path.join(__dirname, 'sensor_2data.html');
    fs.readFile(filePath, 'utf8', (err, data) => {
      if (err) {
        console.error('Failed to read HTML file:', err);
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Internal Server Error');
      } else {
        res.writeHead(200, { 'Content-Type': 'text/html' });
        res.end(data);
      }
    });
  } else if (parsedUrl.pathname === '/sensor_2data.html') {
    // Serve the sensor data HTML page
    const filePath = path.join(__dirname, 'sensor_2data.html');
    fs.readFile(filePath, 'utf8', (err, data) => {
      if (err) {
        console.error('Failed to read HTML file:', err);
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Internal Server Error');
      } else {
        res.writeHead(200, { 'Content-Type': 'text/html' });
        res.end(data);
      }
    });
  } else {
    // If the URL is not handled, return a 404 error
    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end('Not Found');
  }
});

// Define the port on which the server will listen
const port = 3000;
server.listen(port, () => {
  console.log(`Server running at http://localhost:${port}/`);
});








