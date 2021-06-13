$(document).ready(function () {
    var socket = io();     
    socket.on("server-received", function () {
        document.getElementById("Result").innerHTML = "Server request has been received";
    });
    $("#BTN_Submit").click(function () {
        var Kp = parseFloat(document.getElementById("PID_P").value);
        var Ki = parseFloat(document.getElementById("PID_I").value);
        var Kd = parseFloat(document.getElementById("PID_D").value);

        var PID = {
            "P": Kp,
            "I": Ki,
            "D": Kd
        }
        socket.emit("web-send-data", PID);
    })
    $("#BTN_Stop").click(function () {
    document.getElementById("BTN_Stop").innerHTML = document.getElementById("BTN_Stop").value == "Car Stop" ? "Car Run" : "Car Stop";
    document.getElementById("BTN_Stop").value = document.getElementById("BTN_Stop").value == "Car Stop" ? "Car Run" : "Car Stop";
    })

});
