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
    socket.on("Error_number", function (data) {
        console.log(data);
        io.sockets.emit("Error_number_received", data); // io truyen cho tat ca cac thang dang lang nghe
    });
    socket.on("Left_Eng", function (data) { // Su kien cho toc do dong co trai
        console.log(data);
        io.sockets.emit("Left_Eng_received", data);
    });
    socket.on("Right_Eng", function (data) {    // Su kien cho toc do dong co phai
        console.log(data);
        io.sockets.emit("Right_Eng_received", data);
    });
    socket.on("PID_param", function (data) {   //Su kien cho thông số PID ban đầu khi có nguồn điện 
        console.log(data);
    });
    socket.on("STM32_BOOTUP", function (data) {         //Sự kiện khi cả STM32 và ESP bootup lần đầu khi có nguồn điện 
        console.log(data);
        io.sockets.emit("Bootup");
    });
});

app.get("/", function (req, res) {
    res.render("index");
});
