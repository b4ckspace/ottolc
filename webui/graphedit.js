//this file manages the graph editor for managing motor movement over time and provides hlper functions
// for importing and exporting functions

let width = 5000,
    height = 300;

//points for each servos movement. lines[0] is for animation keyframes
//thats why we fix the y-position in mousemove
let lines = [[],[],[],[],[],[],[],[],[]];

var mousepos = [0, 0];
let maybe_changed = false;
let selected_line_idx = 1;

let dragged = null,
    selected = null;

let colors = d3.scale.category10();


let svg = d3.select("#graphedit").append("svg")
    .attr("width", width)
    .attr("height", height)

svg.append("rect")
    .attr("width", width)
    .attr("height", height)
    .attr("fill", "#efefef")
    .on("mousedown", mousedown);
svg.append("text").attr("class", "mousetext")
svg.append("path").datum([[0, height/2], [width, height/2]]).attr("d", d3.svg.line()).style("stroke", "black");
svg.append("path").datum([[0, height], [width, height]]).attr("d", d3.svg.line()).style("stroke", "black");
svg.append("path").datum([[0, 0], [width, 0]]).attr("d", d3.svg.line()).style("stroke", "black");

for (var i = 1; i < lines.length; i++) {
  let line = lines[i];
  svg.append("path")
      .datum(line)
      .attr("class", "line")
      .attr("id", `line${i}`)
      .call(redraw);
}
d3.select(window)
    .on("mousemove", mousemove)
    .on("mouseup", mouseup)
    .on("keydown", keydown);


// svg.node().focus();

function redraw() {
  // fisttspos = lines[0][0]?lines[0][0][0]:50;

  cleanPoints();

  lines[5]=[]
  lines[6]=[]
  lines[7]=[]
  lines[8]=[]
  for(let p of lines[0]){
    lines[5].push(pointAtPathX(1,p[0]))
    lines[6].push(pointAtPathX(2,p[0]))
    lines[7].push(pointAtPathX(3,p[0]))
    lines[8].push(pointAtPathX(4,p[0]))
  }

  svg.selectAll(".mousetext")
            .datum(mousepos)
            .attr("x", function(d) { return d[0]+10; })
            .attr("y", function(d) { return d[1]+10; })
            .text((d)=>{return Math.floor( ((height-d[1])-height/2)/(height/2)*90) })

  for (let i = 0; i < lines.length; i++) {
    let line = lines[i];
    let vline = d3.svg.line();
    let mypath = svg.select(`#line${i}`).attr("d", vline).datum(line);
    if((i>0)&&(i<5)){
      mypath.style("stroke", colors(i))
    }else{
      mypath.style("stroke", "black")
    }

    if(i==0){
      let framelines = svg.selectAll(".animationline").data(line);
      framelines.enter().append("path")
          .attr("class", "animationline");
      framelines.attr("d", (d)=>{
        return d3.svg.line()([[d[0], 0], [d[0], height]])
      });
      framelines.exit().remove();
      let tstexts = svg.selectAll(".tstext").data(line);
      tstexts.enter().append("text").attr("class", "tstext");
      tstexts.exit().remove();
      tstexts.attr("x", function(d) { return d[0]+10; })
            .attr("y", function(d) { return d[1]+10; })
            .text((d)=>{return xposToTs(d[0])})
    }
    if(i>4)
      continue
    let circle = svg.selectAll(`.circle${i}`)
        .data(line, function(d) { return d; });
    ((i)=>{
      circle.enter().append("circle")
          .style("stroke", colors(i))
          .attr("r", 1e-6)
          .attr("class", `circle${i} drag`)
          .on("mousedown", function(d) {
            selected_line_idx=i;
            document.getElementById(`radio${i}`).checked=true;
            selected = dragged = d; redraw(); })
        .transition()
          .duration(750)
          .ease("elastic")
          .attr("r", 6.5);
    })(i)

    circle
        .classed("selected", function(d) { return d === selected; })
        .attr("cx", function(d) { return d[0]; })
        .attr("cy", function(d) { return d[1]; });

    circle.exit().remove();

  }

  drawMPoints();
}

function mousedown() {
  maybe_changed=true;
  let line = lines[selected_line_idx];
  line.push(selected = dragged = d3.mouse(svg.node()));
  line = line.sort(function(a,b){return  a[0]-b[0]})
  redraw();
}

function mousemove() {
  //draw path points here
  let pos = d3.mouse(svg.node());
  if((pos[0]<0)||(pos[0]>width)||(pos[1]<0)||(pos[1]>height))
    return
  mousepos = pos;
  if (dragged){
    maybe_changed=true;
    let m = mousepos;
    dragged[0] = Math.max(0, Math.min(width, m[0]));
    dragged[1] = Math.max(0, Math.min(height, m[1]));
    if(selected_line_idx==0){
      dragged[1]=height/2;
    }
  }
  redraw();
}



function mouseup() {
  if (!dragged) return;
  mousemove();
  dragged = null;
  maybe_changed=true;
}

//support deleting the selected point by pressing the specified keys
function keydown() {
  if (!selected) return;
  let points = lines[selected_line_idx];
  switch (d3.event.keyCode) {
    case 8: // backspace
    case 46: { // delete
      var i = points.indexOf(selected);
      points.splice(i, 1);
      selected = points.length ? points[i > 0 ? i - 1 : 0] : null;
      maybe_changed=true;
      redraw();
      break;
    }
  }
}

//ensure points are not on the same x coordinate and the points are in the right order
function cleanPoints(){
  let min_dist = 0.1

  for(let points of lines){
    //implicitly allows moving several points to the right
    for (var i = 1; i < (points.length); i++) {
      if(points[i][0]<=points[i-1][0]){
        points[i][0]=points[i-1][0]+(min_dist/2);
      }
    }
    //uggly code to allow moving points to the left, not working propperly but better than nothing
    for (var i = points.length-2; i >= 0; i--) {
      if(points[i][0]>=points[i+1][0]-min_dist){
        points[i+1][0]-=min_dist;
        points[i][0]=points[i+1][0]-min_dist*2;
      }
    }

  }
}

// this function interpolates the point at a certain x position based on the paths points
// right before and after the position we are looking fore
// i'm sure this implementation can be improved by a lot!
function pointAtPathX(lineidx, xpos) {
  let before = [-1,height/2];
  let after = [10e10,height/2];
  let line = lines[lineidx];
  for (var i =0; i < line.length; i++) {
    let point = line[i];
    if( (point[0]>before[0])&&(point[0]<=xpos)){
      before = point;
    }
    if((point[0]<after[0])&&(point[0]>xpos)){
      after = point;
    }
  }
  let factor = (xpos-before[0])/(after[0]-before[0]);
  return [xpos, (before[1]+ (after[1]-before[1])*factor )];
}

function drawMPoints(){
  let pts = [1,2,3,4].map((i)=>{
    return pointAtPathX(i, mousepos[0]);
  });

  if(model_ready){
    setLeftLeg(   ((height-pts[0][1])-height/2)/(height/2)*90 );
    setRightLeg(  ((height-pts[1][1])-height/2)/(height/2)*90 );
    setLeftFoot(  ((height-pts[2][1])-height/2)/(height/2)*90 );
    setRightFoot( ((height-pts[3][1])-height/2)/(height/2)*90 );
  }

  let circle = svg.selectAll(".mousecircle")
    .data(pts, function(d) { return d; });
  circle.enter().append("circle")
    .style("stroke", "black")
    .attr("r", 5)
    .attr("class", "mousecircle");
  circle
    .attr("cx", function(d) { return d[0]; })
    .attr("cy", function(d) { return d[1]; });
  circle.exit().remove();
}

//how many px are 1 ms
var tscale = 2;
var fisttspos = 0;
function xposToTs(xpos) {
  return (xpos-fisttspos)*tscale;
}
function tsToXpos(timestamp){
  return (timestamp/tscale)+fisttspos;
}
function exportAnim(){
  //output format:
  //array of strings that represent animation keyframes.
  //keyframe format: &rightFootv, &leftFootv, &rightLegv, &leftLegv, &duration
  //note that our internal representation is absolute timestamps while keyframes take timedeltas
  let ret = []
  let lasttime = 0;
  for(let keyframe of lines[0]){
    let newtime = xposToTs(keyframe[0]);
    let deltatime = Math.round(newtime - lasttime);
    lasttime = newtime;
    let pts = [1,2,3,4].map((i)=>{
      return pointAtPathX(i, keyframe[0]);
    });
    let leftLeg   = Math.round( ((height-pts[0][1])-height/2)/(height/2)*90 );
    let rightLeg  = Math.round( ((height-pts[1][1])-height/2)/(height/2)*90 );
    let leftFoot  = Math.round( ((height-pts[2][1])-height/2)/(height/2)*90 );
    let rightFoot = Math.round( ((height-pts[3][1])-height/2)/(height/2)*90 );
    ret.push(`${rightFoot} ${leftFoot} ${rightLeg} ${leftLeg} ${deltatime}`)
  }
  return ret;
}
function exportCode(){
  //generates code to be put in a 
  let ret = []
  let lasttime = 0;
  for(let keyframe of lines[0]){
    let newtime = xposToTs(keyframe[0]);
    let deltatime = Math.round(newtime - lasttime);
    lasttime = newtime;
    let pts = [1,2,3,4].map((i)=>{
      return pointAtPathX(i, keyframe[0]);
    });
    let leftLeg   = Math.round( ((height-pts[0][1])-height/2)/(height/2)*90 )+90;
    let rightLeg  = Math.round( ((height-pts[1][1])-height/2)/(height/2)*90 )+90;
    let leftFoot  = Math.round( ((height-pts[2][1])-height/2)/(height/2)*90 )+90;
    let rightFoot = Math.round( ((height-pts[3][1])-height/2)/(height/2)*90 )+90;
    ret.push(`prependAnimationFrame(${leftFoot}, ${rightFoot}, ${leftLeg}, ${rightLeg}, ${deltatime});`)
  }

  return ret.reverse().join("\n");
}
function exportAnim(){
  //output format:
  //array of strings that represent animation keyframes.
  //keyframe format: &rightFootv, &leftFootv, &rightLegv, &leftLegv, &duration
  //note that our internal representation is absolute timestamps while keyframes take timedeltas
  let ret = []
  let lasttime = 0;
  for(let keyframe of lines[0]){
    let newtime = xposToTs(keyframe[0]);
    let deltatime = Math.round(newtime - lasttime);
    lasttime = newtime;
    let pts = [1,2,3,4].map((i)=>{
      return pointAtPathX(i, keyframe[0]);
    });
    let leftLeg   = Math.round( ((height-pts[0][1])-height/2)/(height/2)*90 );
    let rightLeg  = Math.round( ((height-pts[1][1])-height/2)/(height/2)*90 );
    let leftFoot  = Math.round( ((height-pts[2][1])-height/2)/(height/2)*90 );
    let rightFoot = Math.round( ((height-pts[3][1])-height/2)/(height/2)*90 );
    ret.push(`${rightFoot} ${leftFoot} ${rightLeg} ${leftLeg} ${deltatime}`)
  }
  return ret;
}

function importAnim(rawdata){
  let full_time = 0;
  let rlines = JSON.parse(rawdata);
  lines = [[],[],[],[],[],[],[],[],[]]
  for(let rline of rlines){
    let els = rline.split(" ");
    let rightFoot = parseInt(els[0], 10)/-90;
    let leftFoot  = parseInt(els[1], 10)/-90;
    let rightLeg  = parseInt(els[2], 10)/-90;
    let leftLeg   = parseInt(els[3], 10)/-90;
    let duration  = parseInt(els[4], 10);
    full_time+=duration;

    let xpos = tsToXpos(full_time);
    lines[0].push([xpos, height/2])
    lines[1].push([xpos, (height/2)+((height/2)*leftLeg)])
    lines[2].push([xpos, (height/2)+((height/2)*rightLeg)])
    lines[3].push([xpos, (height/2)+((height/2)*leftFoot)])
    lines[4].push([xpos, (height/2)+((height/2)*rightFoot)])
  }
  redraw();
  //this fixes the paths not being drawn. we need to figure out the reason sometime
  redraw();
}