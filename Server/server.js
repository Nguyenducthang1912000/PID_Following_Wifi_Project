var express = require("express");
var app = express();

app.use(express.static("./public"));
app.set("view engine", "ejs");
app.set("views", "./views");

var bodyParser = require("body-parser");
app.use(bodyParser.urlencoded({ extended: true }));

// var db = require("./database/database.js");

var server = require("http").Server(app);
var io = require("socket.io")(server);

const port_Listen = 3000;
// server.listen(port_Listen, "192.168.1.250");
server.listen(port_Listen);
io.on("connection", function (socket) {
    console.log("Server: Client connected: " + socket.id);

    socket.on("PID_value_ready", function (data) {
        console.log(data);
        socket.emit("server-received");
        io.sockets.emit("PID_value", data);
    });

    socket.on("State_value_ready", function (data) {
        console.log(data);
        io.sockets.emit("Car_State", data);
    });

    socket.on("Matrix", function (data) {
        console.log(data);
        io.sockets.emit("First_Last", data);
    });
});

app.get("/", function (req, res) {
    res.render("index");
});
