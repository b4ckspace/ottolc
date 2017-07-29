document.getElementById("btnexp").addEventListener('click',()=>{
  document.getElementById("impexpcont").value=JSON.stringify(exportAnim(), null, "    ")
});
document.getElementById("btnimp").addEventListener('click',()=>{
  importAnim(document.getElementById("impexpcont").value);
  maybe_changed = true;
});
document.getElementById("btncode").addEventListener('click',()=>{
  document.getElementById("impexpcont").value=exportCode();
});
document.querySelectorAll(".gsel").forEach((el)=>{
  el.addEventListener('click',function(){selected_line_idx=parseInt(this.value)})
})
for (var i = 0; i < 5; i++) {
  document.querySelector(`label[for=radio${i}]`).style.color=colors(i);
}

function model_loaded(){
  try{
    let res = JSON.parse(atob(location.hash.substring(1)));
    // console.log(res.version);
    if(res.v="0.0.1"){
      lines[0]=res.p[0];
      lines[1]=res.p[1];
      lines[2]=res.p[2];
      lines[3]=res.p[3];
      lines[4]=res.p[4];
      redraw();
      redraw();
    }
  }catch(e){}
}

function updateHash(){
  if(!maybe_changed)
    return
  maybe_changed = false;
  let obj = {v:"0.0.1"}
  obj.p=[];
  obj.p[0]=lines[0];
  obj.p[1]=lines[1];
  obj.p[2]=lines[2];
  obj.p[3]=lines[3];
  obj.p[4]=lines[4];
  console.log("c")
  location.hash = `#${btoa(JSON.stringify(obj))}`
}
setInterval(updateHash, 100);