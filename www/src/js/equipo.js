var source = new EventSource('http://192.168.1.2:9000');
var indice = 0;
var secciones = ['intro', 'one', 'two', 'three'];
var sumador = 1;
source.onmessage = function(event) {
    window.mievento = event;
    console.log('event.data:', event.data);
    var objeto = JSON.parse(event.data);
    console.log('objeto.data:', objeto);
    if (indice < 0){
      indice = secciones.length;
    }
    if (indice >= secciones.length){
      indice = 0;
    }
    window.location.href = "#" + secciones[indice];
    console.log('indice', indice);
    indice = indice + 1;
}
