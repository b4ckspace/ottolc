let width = 960,
    height = 300;

//points for each servos movement. lines[0] is for animation keyframes
//thats why we fix the y-position in mousemove
let lines = [[],[],[],[],[]];

var mousepos = [0, 0];

let selected_line_idx = 1;

let dragged = null,
    selected = null;

let colors = d3.scale.category10();


let svg = d3.select("body").append("svg")
    .attr("width", width)
    .attr("height", height)

svg.append("rect")
    .attr("width", width)
    .attr("height", height)
    .attr("fill", "#efefef")
    .on("mousedown", mousedown);
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


svg.node().focus();

function redraw() {
  cleanPoints();
  for (let i = 0; i < lines.length; i++) {
    let line = lines[i];
    let vline = d3.svg.line();
    if(i>0)
      svg.select(`#line${i}`).attr("d", vline).style("stroke", colors(i));

    if(i==0){
      let framelines = svg.selectAll(".animationline").data(line);
      framelines.enter().append("path")
          .attr("class", "animationline");
      framelines.attr("d", (d)=>{
        return d3.svg.line()([[d[0], 0], [d[0], height]])
      });

      framelines.exit().remove();
    }

    let circle = svg.selectAll(`.circle${i}`)
        .data(line, function(d) { return d; });

    ((i)=>{
      circle.enter().append("circle")
          .style("stroke", colors(i))
          .attr("r", 1e-6)
          .attr("class", `circle${i} drag`)
          .on("mousedown", function(d) { selected_line_idx=i ;selected = dragged = d; redraw(); })
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

  if (d3.event) {
    d3.event.preventDefault();
    d3.event.stopPropagation();
  }

}

function mousedown() {
  let line = lines[selected_line_idx];
  line.push(selected = dragged = d3.mouse(svg.node()));
  line = line.sort(function(a,b){return  a[0]-b[0]})
  redraw();
}

function mousemove() {
  //draw path points here
  mousepos = d3.mouse(svg.node());
  if (dragged){
    let m = mousepos;
    dragged[0] = Math.max(0, Math.min(width, m[0]));
    dragged[1] = Math.max(0, Math.min(height, m[1]));
    if(selected_line_idx==0){
      dragged[1]=height/2;
    }
    redraw();
  }else{
    drawMPoints();
  }
}



function mouseup() {
  if (!dragged) return;
  mousemove();
  dragged = null;
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
  let before = [-10e10,height/2];
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
