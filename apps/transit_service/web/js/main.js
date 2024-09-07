// Important global data for the campus/city environment.
//var socket = new WebSocket("ws://" + location.hostname+(location.port ? ':'+location.port: ''), "web_server");
let api = new WSApi();
var connected = false;
var entities = {};
var entityList = [];
var sceneFile = "scenes/umn.json";
var sceneModel = "assets/model/umn.obj";
var sceneTexture = "assets/texture/umn.png";
var sceneScale = [0.05,0.05,0.05];
var scenePosition = [0,-13,0];
var camera, scene, renderer, controls;
var models = [];
var routes = [];
var paths = {};
var currentView = -1;
var showRoutes = false;
var showPaths = true;
var firstpower = true;

// More important related to models and animation.
var geometry, material, mesh;
var container = document.querySelector( '#scene-container' );
const mixers = [];
const clock = new THREE.Clock();
const loader = new THREE.GLTFLoader();
const objloader = new THREE.OBJLoader();
var simSpeed = 1.0;
// Function definitions start here...

// This is the function that is called once the document is started.
$( document ).ready(function() {
  var simSpeedSlider = document.getElementById("simSpeed");
  
  // Update the current slider value (each time you drag the slider handle)
  simSpeedSlider.oninput = function() {
    simSpeed = this.value / 10.0;
  }

  // Init() starts up the scene and its update loop.
  init();

  // Start checking for when the user resizes their application window.
  window.addEventListener( 'resize', onWindowResize );

  // Listen for when the system wants to create new scene objects.
  try {
    api.onmessage = function(msg, data)  {
      if ("event" in data) {
        if (data.event == "AddEntity") {
          console.log(data.details);
          addEntity(data.details);
        }
        if (data.event == "UpdateEntity") {
          //console.log(data.details);
          var e = data.details;

          if (e.id in entities) {
            var model = entities[e.id];
            model.position.x = e.pos[0];
            model.position.y = e.pos[1];
            model.position.z = e.pos[2];
            
            model.position.x = model.position.x/14.2;
            model.position.y = model.position.y/20.0 - 13.0;
            model.position.z = model.position.z/14.2;

            model.position.x += model.offset.x;
            model.position.y += model.offset.y;
            model.position.z += model.offset.z;

            var dir = new THREE.Vector3(e.dir[0], e.dir[1], e.dir[2]);
            var pos = new THREE.Vector3();
            pos.addVectors(dir, model.position);

            var vector = new THREE.Vector3( 0, 1, 0 );
            vector = model.worldToLocal(vector);

            if(e.color) {
              model.children[0].traverse((o) => {
                if(o.isMesh) {
                  c = o.userData.defaultColor.clone();
                  c.multiply(new THREE.Color(e.color));
                  o.material.color.set(c);
                }
              });
            } else {
              model.children[0].traverse((o) => {
                if(o.isMesh) {
                  o.material.color.set(o.userData.defaultColor);
                }
              })
            }

            var adjustedDirVector = model.localToWorld(new THREE.Vector3(0,0,0)).add(dir);
            model.lookAt(adjustedDirVector);
          }

          if (currentView >= 0) {
            controls.target.copy(entities[currentView].position);
            controls.update();
          }
        }
        if (data.event == "RemoveEntity") {
          //console.log(data);
          removeEntity(data.details.id);
        }  
        if (data.event == "observe") {
          displayNotification(data.details);
        }
        if (data.event == "power") {
          if(firstpower == true){
            displayNotification(data.details);
            firstpower = false;
          } 
          else{
            notifbar = document.getElementById("notification-bar");
            notifbar.textContent = notifbar.textContent.replace(/power:\s*\d+/, data.details.info);
          }
        }
      }
    }
  }
  catch(exception) {
    alert('<p>Error' + exception);
  }

  loadScene(sceneFile);
});

/*// This function is triggered once the web socket is opened.
socket.onopen = function() {
  socket.send(JSON.stringify({command: "test"}));
  loadScene(sceneFile);
  connected = true;
}*/

// This function defines the properties of the scene as well as starts the
// update loop.
function init() {
  const fov = 35; // fov = Field Of View
  const aspect = container.clientWidth / container.clientHeight;
  const near = 0.1;
  const far = 1000;

  camera = new THREE.PerspectiveCamera( fov, aspect, near, far );
  camera.position.set( -10, 10, 10 );
  controls = new THREE.OrbitControls( camera, container );

  scene = new THREE.Scene();
  scene.background = new THREE.Color( 'skyblue' );

  const textureLoader = new THREE.TextureLoader();

  const texture = textureLoader.load( sceneTexture );
  // set the "color space" of the texture
  texture.encoding = THREE.sRGBEncoding;
  // reduce blurring at glancing angles
  texture.anisotropy = 16;
  // create a Standard material using the texture we just loaded as a color map
  material = new THREE.MeshStandardMaterial( { map: texture } );

  geometry = new THREE.BoxBufferGeometry( 2, 2, 2 );
  mesh = new THREE.Mesh( geometry, material );

  geometry = new THREE.PlaneBufferGeometry( 100, 100, 100 );
  mesh = new THREE.Mesh( geometry, material );
  mesh.rotation.x = -3.14159/2.0;

  const ambientLight = new THREE.AmbientLight( 0xffffff, 1 );
  scene.add( ambientLight );
  const light = new THREE.DirectionalLight( 0xffffff, 1 );
  light.position.set( 10, 10, 10 );
  scene.add( ambientLight, light );
  const light2 = new THREE.DirectionalLight( 0xffffff, 1 );
  light2.position.set( 0, 10, -10 );
  scene.add( ambientLight, light2 );

  renderer = new THREE.WebGLRenderer( { antialias: true } );
  renderer.setSize( window.innerWidth, window.innerHeight );
  document.body.appendChild( renderer.domElement );
  // start the animation/render loop
  renderer.setAnimationLoop( () => {
    update();
    render();
  });
}

function changeView() {
  currentView = $("#entitySelect").val();
  if (currentView >= 0) {
    controls.target.copy(entities[currentView].position);
    controls.update();
  }
}

// This function builds the initial campus/city scene.
function loadScene(file, initialScene = true) {
  sceneFile = file;
  $.getJSON(sceneFile, function(json) {
    console.log(json);
    for (var i = 0; i < json.length; i++) {
      var command = json[i];
      console.log(command);
      if (command.command == "SetScene") {
        console.log(command);
        sceneModel = command.params.mesh;
        sceneTexture = command.params.texture;
        sceneScale = command.params.scale;
        scenePosition = command.params.position;
      }
      if (command.command == "AddMesh") {
        addMesh(command.params);
      }
      if (command.command == "CreateEntity") {
        api.sendCommand(command.command, command.params);
      }
    }
    loadModels();
    /*if (initialScene) {
      socket.send(JSON.stringify({command: "runScript", "init":true, "script": json}));
    }
    else {
      socket.send(JSON.stringify({command: "runScript", "script": json}));
    }*/
  });
}
var msg = "";
function displayNotification(data) {
  notifbar = document.getElementById("notification-bar");
  // if(msg != data.info) {
  notifbar.textContent += data.info;
    // msg = data.info;
  // }
}

function displayJSON(data) {
  //data should be a standard JSON-style object
  if (data["command"] == "notification") {
    if (data["value"] == 'updateDetails') {
      if (data["details"].color != undefined) {
        entities[data["id"]].children[1].material.color.setHex(parseInt(data["details"].color, 16));
        entities[data["id"]].children[1].material.opacity = 0.5;
      }
      else {
        entities[data["id"]].children[1].material.opacity = 0.0;
      }
      return;
    }

    type_lookup = {
      'scheduled': ' scheduled.',
      'delivered': ' delivered to customer.',
      'en route': ' picked up.',
      'moving': ' now moving.',
      'idle': ' stopped moving.'
    }
    string_ending = type_lookup[data["value"]];
    additional_string = "Entity #" + data["id"] + string_ending + "\r\n";
    notifbar = document.getElementById("notification-bar");
    notifbar.textContent += additional_string;

    const entityId = data["id"];

    if (data["value"] == 'idle' || data["value"] == 'moving') {
      if (entityId in paths) {
        scene.remove(paths[entityId]);
        delete paths[entityId];
      }
    }
    if (data["value"] == 'idle') {
      for ( var mixer of mixers ) {
        if (entityId == mixer.id) {
          mixer.duration = 0;
        }
      }
    }
    else if (data["value"] == 'moving') {
      if ("path" in data) {
        //create a blue LineBasicMaterial
        var material = new THREE.LineBasicMaterial( { color: 0xf0fc03 } );
        const points = [];
        for (var point of  data["path"]) {
          points.push( new THREE.Vector3( point[0], point[1], point[2] ) );
        }
        /*points.push( new THREE.Vector3( - 10, 0, 0 ) );
        points.push( new THREE.Vector3( 0, 10, 0 ) );
        points.push( new THREE.Vector3( 10, 0, 0 ) );*/
        const geometry = new THREE.BufferGeometry().setFromPoints( points );

        const line = new THREE.Line( geometry, material );
        //console.log(routes);
        if (routes.length > 0) {
          line.position.copy( routes[0].position );
          line.scale.copy( routes[0].scale );
          line.material.color.setHex(routes[0].material.color.getHex());
        }

        //"position": [-0.0,-12.5,-0.0],
        //      "scale": [0.0705,0.05,0.0705],
        paths[entityId] = line;
        scene.add( line );
      }

      for ( var mixer of mixers ) {
        if (entityId == mixer.id) {

          mixer.duration = 2;
        }
      }
    }
  }
}

// This function is a helper for loadScene().
function loadModels() {
  // instantiate a loader
  var loader2 = new THREE.OBJLoader();

  // load a resource
  loader2.load(
    // resource URL
    sceneModel,
    // called when resource is loaded
    function ( object ) {
      object.position.copy( new THREE.Vector3( scenePosition[0], scenePosition[1], scenePosition[2]) )
      object.scale.copy( new THREE.Vector3( sceneScale[0], sceneScale[1], sceneScale[2]));
      
      const textureLoader = new THREE.TextureLoader();
      const texture = textureLoader.load( sceneTexture + "-ground.png" );
      texture.encoding = THREE.sRGBEncoding;
      texture.anisotropy = 16;
      var objmaterial = new THREE.MeshStandardMaterial( { map: texture } );

      const texture2 = textureLoader.load(sceneTexture );
      texture.encoding = THREE.sRGBEncoding;
      texture.anisotropy = 16;

      object.traverse( function ( node ) {
          if ( node.name == "EXPORT_GOOGLE_SAT_WM" ) {
            node.material = objmaterial;
          }
          else if ( node.name == "Areas:building" ) {
          var material = new THREE.MeshStandardMaterial( {color: 0x85868f, map: texture2});
          node.material = material;
          }
          else if (!node.isGroup) {
            node.visible = false;
          }
          //node.geometry.computeBoundingBox();
          // max: Object { x: 2249.523193359375, y: 286.9197998046875, z: 1261.8768310546875 }
          // min: Object { x: -2030.950927734375, y: 220.99996948242188, z: -1184.1085205078125 }
          console.log(node);
        } );

        models.push(object);
      scene.add( object );
    },
    // called when loading is in progresses
    function ( xhr ) {
      console.log( ( xhr.loaded / xhr.total * 100 ) + '% loaded' );
    },
    // called when loading has errors
    function ( error ) {
      console.log( 'An error happened', error);
    }
  );
}

function toggleRoutes() {
  showRoutes = !showRoutes;
  for (var routeNum = 0; routeNum < routes.length; routeNum++) {
    routes[routeNum].visible = showRoutes;
  }
}

function togglePaths() {
  showPaths = !showPaths;
  for (const key in paths) {
    paths[key].visible = showPaths;
  }
}

// This function is a helper for loadScene().
function addMesh(obj) {
  // instantiate a loader
  var loader2 = new THREE.OBJLoader();

  // load a resource
  loader2.load(
    // resource URL
    obj.mesh,
    // called when resource is loaded
    function ( object ) {
      //object.position.copy( new THREE.Vector3( scenePosition[0]+1.5, scenePosition[1]+0.5, scenePosition[2]+1.5) )
      object.position.copy( new THREE.Vector3( obj.position[0], obj.position[1], obj.position[2]) )
      object.scale.copy( new THREE.Vector3( obj.scale[0], obj.scale[1], obj.scale[2]) ) //*1.41

      object.traverse( function ( node ) {
        //var material = new THREE.MeshStandardMaterial( {color: 0x8589a1});
        var material = new THREE.LineBasicMaterial( {
          color: parseInt(obj.color, 16), //0xff00e1,
          linewidth: 1,
          linecap: 'round', //ignored by WebGLRenderer
          linejoin:  'round' //ignored by WebGLRenderer
        } );
        node.material = material;
        //node.position.copy( new THREE.Vector3( 30, 10, 35.0) );
        //node.scale.copy( new THREE.Vector3( 1.41, 1.0, 1.41) );
        console.log(node);
        
      } );

      if (obj.type == "route") {
        object.visible = showRoutes;
        routes.push(object);
      }
        models.push(object);
      scene.add( object );
    },
    // called when loading is in progresses
    function ( xhr ) {
      console.log( ( xhr.loaded / xhr.total * 100 ) + '% loaded' );
    },
    // called when loading has errors
    function ( error ) {
      console.log( 'An error happened', error);
    }
  );
}

// A reusable function to set up the models. We're passing in a position
// parameter so that they can be individually placed around the scene.
// Currently, this only works for GLTF/glb formats.
const onLoad = ( gltf, position, scale, start, duration, details, id ) => {
  const model = gltf.scene.children[ 0 ];
  model.scale.copy( scale );

  const animation = gltf.animations[ 0 ];
  console.log(animation);
  
  if (!(typeof animation === "undefined")) {
	const mixer = new THREE.AnimationMixer( model );
    mixers.push( {id: id, mixer: mixer, start: start, duration: duration} );
  var action = mixer.clipAction(animation);
  	action.play();
  }

  var sphereGeom =  new THREE.SphereGeometry( 40, 32, 16 );
  var opacity = details.color == undefined ? 0.0 : 0.5;
  var sphereMaterial = new THREE.MeshBasicMaterial( { color: parseInt(details.color, 16), transparent: true, opacity: opacity } );
  var sphere = new THREE.Mesh( sphereGeom, sphereMaterial );
  sphere.scale.copy(new THREE.Vector3(0.03, 0.03, 0.03));

  const group = new THREE.Group();
  group.add( model );
  group.add(sphere);
  group.position.copy(position);
  
  if (details.offset != undefined) {
    group.offset = new THREE.Vector3(details.offset[0], details.offset[1], details.offset[2]);
  }
  else {
    group.offset = new THREE.Vector3(0.0, 0.0, 0.0);
  }

  group.traverse((o) => {
    if(o.isMesh) {
      o.userData.defaultColor = o.material.color.clone();
    }
  });

  models.push(group);
  scene.add( group );
  entities[id] = group;
  entityList.push(id);
};

// This function is called whenever a new object needs to be added to the scene.
function addEntity(data) {
  $("#entitySelect").append($('<option value="' + data.id + '">' + data.details.name + '</option>'));
  // the loader will report the loading progress to this function
  const onProgress = () => {};
  // the loader will send any error messages to this function, and we'll log
  // them to to console
  const onError = ( errorMessage ) => { console.log( errorMessage ); };
  // Choose a locally-stored gltf/glb model and attempt to load it.
  var position = new THREE.Vector3(data.details.position[0], data.details.position[1], data.details.position[2]);
  
  position.x = position.x/14.2;
  position.y = position.y/20.0 - 13.0;
  position.z = position.z/14.2;

  console.log(data.details);

  var scale = new THREE.Vector3(data.details.scale[0], data.details.scale[1], data.details.scale[2]);
  var id = data.id;
  loader.load( data.details.mesh, gltf => onLoad( gltf, position, scale, data.details.start, data.details.duration, data.details, id ), onProgress, onError );
}

function removeEntity(id) {
  console.log(models);
  var model = entities[id];
  models = models.filter(function(value, index, arr){ return value != model;});
  scene.remove( model );
  $("#entitySelect option[value='" + id + "']").remove();
  delete entities[id];
  if (currentView == id) {
    currentView = -1;
  }
  console.log(models);
}

/*// This function kills the webpage's socket connection.
function kill() {
  if (connected) {
    socket.send(JSON.stringify({command: "kill"}));
  }
}*/

var time = 0.0;

// This function updates the scene's animation cycle.
function update() {
  // Get the time since the last animation frame.
  const delta = clock.getDelta();
  time += delta;

  // Iterate through and update the animation mixers for each object in the
  // scene.
  for ( const mixer of mixers ) {
    if (mixer.start == undefined || mixer.duration == undefined) {
      mixer.mixer.update(delta);
    }
    else {
      var newTime = time - mixer.start;
      var iterations = Math.floor(newTime/mixer.duration);
      newTime = newTime - iterations*mixer.duration + mixer.start;
      mixer.mixer.setTime(newTime);
    }
  }

  // Send the update command to the socket.
  //if (connected) {
    //socket.send(JSON.stringify({command: "update", delta: delta}));
    //socket.send(JSON.stringify({command: "update", delta: delta, simSpeed: simSpeed}));
    api.sendCommand("Update", { simSpeed: simSpeed });
  //}
}

// This function simply renders the scene based on the camera position.
function render() {
  renderer.render( scene, camera );
}

// This function updates the projection matrix and renderer whenever the
// user's application window is resized.
function onWindowResize() {
  // set the aspect ratio to match the new browser window aspect ratio
  camera.aspect = container.clientWidth / container.clientHeight;

  // update the camera's frustum
  camera.updateProjectionMatrix();

  // update the size of the renderer AND the canvas
  renderer.setSize( container.clientWidth, container.clientHeight );
}
