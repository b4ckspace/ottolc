var scene = new THREE.Scene();
var camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );

var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );

var objects = []
let raycaster = new THREE.Raycaster();
var loader = new THREE.STLLoader();

for(const lpos of [[100,200,100],[-100, 200, -100],[0, -200, 0]]){
	const pointLight =new THREE.PointLight(0xFFFFFF);
	pointLight.position.x = lpos[0];
	pointLight.position.y = lpos[1];
	pointLight.position.z = lpos[2];
	var pointLightHelper = new THREE.PointLightHelper( pointLight, 1);
	scene.add( pointLightHelper);
	scene.add(pointLight);
}


var limbs = {};
var lmash;
loader.load( 'assets/3D print/OTTO_footL_v4.stl', function ( geometry ) {
	var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
	var lf = new THREE.Mesh( geometry, material );
	lf.rotation.set(-0.5*Math.PI, 0, 0.5*Math.PI);
	lf.position.x=-35;
	objects.push(lf);
	limbs.lf=lf;
	var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
	var rf = new THREE.Mesh( geometry, material );
	rf.rotation.set(-0.5*Math.PI, 0, -0.5*Math.PI);
	rf.position.x=35;
	
	objects.push(rf);
	limbs.rf=rf;
	loader.load( 'assets/3D print/OTTO_leg_v4.stl', function ( geometry ) {
		var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
		var ll = new THREE.Mesh( geometry, material );
		objects.push(ll);
		ll.rotation.set(Math.PI,0,Math.PI);
		ll.position.set(0, -10, 45);
		lf.add(ll);
		limbs.ll=ll;
		var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
		var rl = new THREE.Mesh( geometry, material );
		objects.push(rl);
		rl.rotation.set(Math.PI,0,Math.PI);
		rl.position.set(0, -10, 45);
		rf.add(rl);
		limbs.rl=rl;
		loader.load( 'assets/3D print/OTTO_body_v3.stl', function ( geometry ) {
			var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
			var body = new THREE.Mesh( geometry, material );
			body.rotation.set(-0.5*Math.PI,0,0.5*Math.PI)
			objects.push(body);
			body.position.y=45;
			lmash=body;

			scene.add(lf);
			scene.add(rf);
			scene.add(body);
		});
	} );
} );



var axisHelper = new THREE.AxisHelper(100);
scene.add( axisHelper );
let controls = new THREE.OrbitControls( camera, renderer.domElement );
camera.position.set(45,52,77);
function animate() {
	requestAnimationFrame( animate );
	controls.update();
	renderer.render( scene, camera );
}
requestAnimationFrame(animate);

function onWindowResize() {
	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();
	renderer.setSize( window.innerWidth, window.innerHeight );
}
window.addEventListener( 'resize', onWindowResize, false );

function onDocumentMouseDown( event ) {
	event.preventDefault();
	let mouse = {};
	mouse.x = ( event.clientX / renderer.domElement.clientWidth ) * 2 - 1;
	mouse.y = - ( event.clientY / renderer.domElement.clientHeight ) * 2 + 1;
	raycaster.setFromCamera( mouse, camera );
	var intersects = raycaster.intersectObjects( objects );

	if ( intersects.length > 0 ) {
		intersects[ 0 ].object.material.color.setHex( Math.random() * 0xffffff );
	}
}
document.addEventListener( 'mousedown', onDocumentMouseDown, false );
