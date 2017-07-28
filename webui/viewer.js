var scene = new THREE.Scene();

let viewerheight = 300;
let viewerwidth  = 300;

var camera = new THREE.PerspectiveCamera( 75, viewerheight / viewerwidth, 0.1, 1000 );

var renderer = new THREE.WebGLRenderer();
renderer.setSize( viewerwidth, viewerheight );
document.body.appendChild( renderer.domElement );

var objects = []
let raycaster = new THREE.Raycaster();
var loader = new THREE.STLLoader();

for(const lpos of [[150,200,100],[-100, 200, -150],[0, -200, 0]]){
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
var model_ready= false;
loader.load( 'assets/3D print/OTTO_leg_v4.stl', function ( geometry ) {
	var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
	var ll = new THREE.Mesh( geometry, material );
	objects.push(ll);
	ll.rotation.set(Math.PI,0,Math.PI);
	ll.position.set(0, 25, 0);
	limbs.ll=ll;
	var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
	var rl = new THREE.Mesh( geometry, material );
	objects.push(rl);
	rl.rotation.set(Math.PI,0,Math.PI);
	rl.position.set(0, -25, 0);
	limbs.rl=rl;




	loader.load( 'assets/3D print/OTTO_footL_v4.stl', function ( geometry ) {
		var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
		var lf = new THREE.Mesh( geometry, material );
		lf.rotation.set(Math.PI, 0, Math.PI);
		var lfp = new THREE.Object3D();
		lfp.position.z = 50-20;
		lfp.position.y = 0;
		lf.position.z=15;
		lf.position.y=10;
		lfp.add(lf);
		objects.push(lf);
		limbs.lf=lf;
		limbs.lfp=lfp;

		var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
		var rf = new THREE.Mesh( geometry, material );
		rf.rotation.set(Math.PI, 0, 0);
		var rfp = new THREE.Object3D();
		rfp.position.z = 50-20;
		rfp.position.y = 0;
		rf.position.z=15;
		rf.position.y=-10;
		rfp.add(rf);
		objects.push(rf);
		limbs.rf=rf;
		limbs.rfp=rfp;


		loader.load( 'assets/3D print/OTTO_body_v3.stl', function ( geometry ) {
			var material = new THREE.MeshPhongMaterial( { color: Math.random() * 0xffffff} );
			var body = new THREE.Mesh( geometry, material );
			body.rotation.set(-0.5*Math.PI,0,0.5*Math.PI)
			objects.push(body);
			body.position.y=45;
			limbs.body=body;
			lmash=body;

			body.add(ll);
			body.add(rl);
			ll.add(lfp);
			rl.add(rfp);
			scene.add(body);

			model_ready = true;
			// var gui = new dat.GUI();
			// var lg = gui.addFolder('left');
			// lg.add(limbs.ll.rotation, 'z').step(0.01);
			// lg.add(lfp.rotation, 'x').step(0.01);
			// var rg = gui.addFolder('right');
			// rg.add(limbs.rl.rotation, 'z').step(0.01);
			// rg.add(rfp.rotation, 'x').step(0.01);
		});
	});
});



var axisHelper = new THREE.AxisHelper(100);
scene.add( axisHelper );
let controls = new THREE.OrbitControls( camera, renderer.domElement );
camera.position.set(65,140,113);
function animate() {
	requestAnimationFrame( animate );
	controls.update();
	renderer.render( scene, camera );
}
requestAnimationFrame(animate);

function onWindowResize() {
	camera.aspect = viewerwidth / viewerheight;
	camera.updateProjectionMatrix();
	renderer.setSize( viewerwidth, viewerheight);
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

function setLeftLeg(angle){
	limbs.ll.rotation.z = Math.PI + angle/90*0.5*Math.PI;
}
function setRightLeg(angle){
	limbs.rl.rotation.z = Math.PI + angle/90*-0.5*Math.PI;
}
function setLeftFoot(angle){
	limbs.lfp.rotation.x=Math.PI*0.5*angle/90
}
function setRightFoot(angle){
	limbs.rfp.rotation.x=Math.PI*0.5*angle/90
}