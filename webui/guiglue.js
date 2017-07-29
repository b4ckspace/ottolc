document.getElementById("btnexp").addEventListener('click',()=>{
  document.getElementById("impexpcont").value=JSON.stringify(exportAnim(), null, "    ")
});
document.getElementById("btnimp").addEventListener('click',()=>{
  importAnim(document.getElementById("impexpcont").value);
});