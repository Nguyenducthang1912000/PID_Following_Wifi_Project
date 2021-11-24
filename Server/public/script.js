$(document).ready(function () {
    var socket = io();
    socket.emit("abc", "hello");
    socket.on("server-received", function () {
        document.getElementById("Result").innerHTML = "Server request has been received";
    });
    socket.on("Error_number_received", function (data) {
        document.getElementById("Error_display").value = data;
    });
    socket.on("Left_Eng_received", function (data) {
        document.getElementById("Left_Eng_display").value = data;
    });
    socket.on("Right_Eng_received", function (data) {
        document.getElementById("Right_Eng_display").value = data;
    });
    socket.on("Bootup", function () {
        document.getElementById("BTN_Stop").value = "Car Run";
        document.getElementById("BTN_Stop").innerHTML = "Car Run";
    })
    $("#BTN_Submit").click(function () {
        var Kp = parseFloat(document.getElementById("PID_P").value);
        var Ki = parseFloat(document.getElementById("PID_I").value);
        var Kd = parseFloat(document.getElementById("PID_D").value);

        if (Kp == Ki == Kd == null) {
            Kp = Ki = Kd = 0;
        }

        var PID = {
            "P": Kp,
            "I": Ki,
            "D": Kd
        }
        socket.emit("PID_value_ready", PID);
    })
    $("#BTN_Stop").click(function () {
        var State;
        document.getElementById("BTN_Stop").innerHTML = document.getElementById("BTN_Stop").value == "Car Stop" ? "Car Run" : "Car Stop";
        document.getElementById("BTN_Stop").value = document.getElementById("BTN_Stop").value == "Car Stop" ? "Car Run" : "Car Stop";

        if (document.getElementById("BTN_Stop").value == "Car Stop")
            State = 0;
        else if (document.getElementById("BTN_Stop").value == "Car Run")
            State = 1;

        var Car_state = {
            "Car_state": State
        }
        socket.emit("State_value_ready", Car_state);
    })
    $("#BTN_Submit_Point").click(function () {
        console.log("click");
        var First_point = parseInt(document.getElementById("First_point_id").value);
        var Last_point = parseInt(document.getElementById("Last_point_id").value);

        if (First_point == Last_point == null) {
            First_point = Last_point = 0;
        }

        var Matrix = {
            "First": First_point,
            "Last": Last_point
        }
        socket.emit("Matrix", Matrix);
    })

});
