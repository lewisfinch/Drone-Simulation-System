// Web Sockets API for communication with the backend
let api = new WSApi();

var trip = [];

var helicopterID = 1;

// This method is called when the document is loaded
$(document).ready(function () {
    const map = $('#map');
    // hide the popup
    $("#popup").hide();

    // register a mouse click event that adds circles on an image
    map.click(function (e) {
        var posX = e.pageX - $(this).offset().left;
        var posY = e.pageY - $(this).offset().top;
        if (trip.length < 2) {
            map.append('<circle cx="' + posX + '" cy="' + posY + '" r="10" stroke="green" stroke-width="4" fill="yellow" />');
            map.html($("#map").html());
            trip.push([posX, posY]);
        } else {
            map.empty();
            map.append('<circle cx="' + trip[0][0] + '" cy="' + trip[0][1] + '" r="10" stroke="green" stroke-width="4" fill="yellow" />');
            // var posX = e.pageX - $(this).offset().left;
            // var posY = e.pageY - $(this).offset().top;
            map.append('<circle cx="' + posX + '" cy="' + posY + '" r="10" stroke="green" stroke-width="4" fill="yellow" />');
            map.html($("#map").html());
            trip.insert(1, [[posX, posY]]);
        }
    });
});

// Web sockets event callback
api.onmessage = function (msg, data) {
    // handles events
    if ("event" in data) {
        console.log(data);
        if (data.event == "TripScheduled") {
            $("#popup").show();
            $("#popup").fadeOut(3000);
        }
    }
}


// This function schedules a trip and sends the command to the web sockets api.
function schedule() {
    var errorDiv = document.getElementById("nameError");
    var searchStrat = document.getElementById("search-strategy").value;
    errorDiv.innerHTML = "";
    var name = $("#name").val();
    if (name == "") {
        alert("Error, missing name...");
        $(`.name-input`)
            .addClass('warn')
            .attr("placeholder", "[!] Missing Names [!]");
    }
    if (trip.length < 2) {
        alert("Error, missing pickup and drop off location...");
        $(`.map-container`).addClass('warn');
    }
    if (name != "" && trip.length >= 2) {
        // Clean the warning
        $(`.name-input`)
            .removeClass('warn')
            .attr("placeholder", "Enter your trip name");
        $(`.map-container`).removeClass('warn');

        const map = $('#map');
        var width = map.width();
        var height = map.height();
        var start = [trip[0][0] / width, 1.0, trip[0][1] / height];
        var end = [trip[trip.length - 1][0] / width, 1.0, trip[trip.length - 1][1] / height];
        console.log(start);
        var scale = 0.705;
        var min = {x: -2030.950927734375, y: 220.99996948242188, z: -1184.1085205078125};
        var max = {x: 2249.523193359375, y: 286.9197998046875, z: 1261.8768310546875};

        api.sendCommand("CreateEntity", {
            "type": "robot",
            "name": name,
            "mesh": "assets/model/robot.glb",
            "position": [(min.x + (max.x - min.x) * start[0]) * scale, 254.665 * start[1], (min.z + (max.z - min.z) * start[2]) * scale],
            "scale": [0.25, 0.25, 0.25],
            "direction": [1, 0, 0],
            "speed": 30.0,
            "radius": 1.0,
            "rotation": [0, 0, 0, 0]
        });
        api.sendCommand("ScheduleTrip", {
            name: name,
            start: [trip[0][0], trip[0][1]],
            end: [(min.x + (max.x - min.x) * end[0]) * scale, 254.665 * end[1], (min.z + (max.z - min.z) * end[2]) * scale],
            search: searchStrat
        });
        var details = name + ": ";
        for (var i = 0; i < trip.length; i++) {
            if (i != 0) {
                details += " ---> ";
            }
            details += "(" + trip[i][0].toFixed(1) + ", " + trip[i][1].toFixed(1) + ")";

        }
        $("#list").append("<p class='trip'>" + details + "</p>");

        // reset the trip
        trip = [];
        map.html("");
        $("#name").val("");
    }
}

var humanID = 1;

// function to add humans
function addHuman() {
    // Only add one human every 2 seconds
    let humanBtn = document.getElementById('add-human-btn');
    humanBtn.disabled = true;
    setTimeout(function () {
        humanBtn.disabled = false;
    }, 2000);

    api.sendCommand("CreateEntity", {
        "type": "human",
        "name": "Human-" + humanID,
        "mesh": "assets/model/human.glb",
        "position": [700, 290, 400],
        "scale": [0.005, 0.005, 0.005],
        "rotation": [0, 0, 0, 0],
        "direction": [1, 0, 0],
        "speed": 10.0,
        "radius": 1.0,
        "start": 2.0,
        "duration": 2.0,
        "offset": [0, -0.5, 0]
    });
    console.log("Human-", humanID, " is added.\n");
    humanID++;
}

var ufoAdded = 0;

// Function to add UFOs to the map
function addUFO() {
    if (!ufoAdded) {
        // Only add one UFO
        api.sendCommand("CreateEntity", {
            "type": "UFO",
            "name": "UFO",
            "mesh": "assets/model/UFO.glb",
            "position": [498.292, 600, -228.623],
            "scale": [0.5, 0.5, 0.5],
            "rotation": [0, 0, 0, 0],
            "direction": [1, 0, 0],
            "speed": 65.0,
            "offset": [0, 1.5, 0]
        });
        ufoAdded = 1;
    } else {
        alert("Only one UFO allowed.");
    }
}
