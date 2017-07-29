document.getElementById("btnexp").addEventListener('click',()=>{
  document.getElementById("impexpcont").value=JSON.stringify(exportAnim(), null, "    ")
});
document.getElementById("btnimp").addEventListener('click',()=>{
  importAnim(document.getElementById("impexpcont").value);
});
document.querySelectorAll(".gsel").forEach((el)=>{
  el.addEventListener('click',function(){selected_line_idx=parseInt(this.value)})
})