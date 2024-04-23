const express = require('express');
const mqtt = require('mqtt');
const mongoose = require('mongoose');
const path = require('path');
const WebSocket = require('ws');
const DataModel = require(path.join(__dirname, 'models', 'DataModel'));

// Connexion à MQTT
const client = mqtt.connect('mqtt://192.168.0.182:1883');
client.on('connect', () => {
    client.subscribe('esp8266/mq135');
});

// Connexion à MongoDB
mongoose.connect('mongodb://localhost/mqtt', { useNewUrlParser: true });

// Création d'une nouvelle instance du modèle de données
const newData = new DataModel();

// Créer un serveur WebSocket
const wss = new WebSocket.Server({ port: 3030 });

// Écouter les connexions WebSocket
wss.on('connection', function connection(ws) {
    console.log('Client connected');

    // Envoyer les données initiales au client WebSocket lors de la connexion
    ws.send(JSON.stringify(newData));
});

// Écoute des messages MQTT
client.on('message', function (topic, message) {
  // Convertir le message brut en chaîne de caractères
  const messageString = message.toString();

  // Extraire la valeur du capteur à partir du message
  const sensorValueIndex = messageString.indexOf(':') + 1;
  const sensorValue = messageString.substring(sensorValueIndex).trim();

  // Créer un objet JSON avec la valeur du capteur
  const data = {
      field1: "niveau de gaz: ",
      value: parseInt(sensorValue)
  };

  // Enregistrer les nouvelles données dans MongoDB
  const newData = new DataModel({
      field1: data.field1,
      value: data.value
  });

  // Sauvegarde des données dans MongoDB
  newData.save()
      .then(() => {
          console.log('Données insérées avec succès dans MongoDB');
      })
      .catch(err => {
          console.error('Erreur lors de insertion des données dans MongoDB : ', err);
      });

  // Envoyer les données mises à jour aux clients WebSocket connectés
  wss.clients.forEach(function each(client) {
      if (client.readyState === WebSocket.OPEN) {
          client.send(JSON.stringify(data));
      }
  });

  // Utilisez les données après les avoir mises à jour
  console.log("Valeurs du capteur mises à jour : ", data);
});


// Initialiser le serveur Express
const app = express();
const PORT = 3000;

// Servir la page HTML pour afficher les valeurs du capteur
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

// Démarrer le serveur Express
app.listen(PORT, () => {
    console.log(`Serveur backend en cours d'exécution sur le port ${PORT}`);
});

