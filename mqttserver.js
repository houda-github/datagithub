const mongoose = require('mongoose');

// Configuration de MongoDB
const mongoURL = 'mongodb://localhost:27017/sensor'; // URL de connexion MongoDB

// Connexion à MongoDB
mongoose.connect(mongoURL, {
    useNewUrlParser: true,
    useUnifiedTopology: true
})
.then(() => {
    console.log('Connecté à MongoDB');

    // Créer un modèle Mongoose pour les données
    const DataModel = mongoose.model('Data', {
        timestamp: Date, // Horodatage du message
        data: String // Données du capteur
    });

    // Configuration du broker MQTT
    const mosca = require('mosca');

    const mqttSettings = {
        port: 1883 // Port du broker MQTT
    };

    // Création du serveur MQTT
    const mqttServer = new mosca.Server(mqttSettings);

    // Événement lorsque le serveur MQTT est prêt
    mqttServer.on('ready', () => {
        console.log("Broker Mosca démarré sur le port", mqttSettings.port);
    });

    // Événement lorsqu'un client MQTT se connecte
    mqttServer.on('clientConnected', (client) => {
        console.log('Client connecté:', client.id);
    });

    // Événement lorsqu'un message MQTT est publié
    mqttServer.on('published', (packet, client) => {
        if (packet.topic.indexOf('$SYS') === -1) {
            const payload = packet.payload.toString(); // Convertir la charge utile en chaîne de caractères
            console.log('Message publié:', payload);

            // Insérer les données dans MongoDB
            const newData = new DataModel({
                timestamp: new Date(),
                data: payload
            });

            newData.save()
                .then(() => console.log('Données insérées dans MongoDB'))
                .catch(err => console.error('Erreur lors de l\'insertion des données dans MongoDB:', err));
        }
    });
})
.catch(err => console.error('Échec de la connexion à MongoDB:', err));
