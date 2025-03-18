import ApexCharts from 'https://cdn.jsdelivr.net/npm/apexcharts@3.40.0/dist/apexcharts.esm.js';

let seccionGraficaAgua = document.querySelector('.grafica-nivel-agua');
let seccionGraficaPrecipitacion = document.querySelector('.grafica-nivel-precipitaciones');
let alarmaActuador = document.querySelector('.actuador-alarma');

alarmaActuador.addEventListener('click', () => {
    let spanTexto = alarmaActuador.querySelector('span');
    let pTexto = alarmaActuador.querySelector('p');

    if (alarmaActuador.classList.contains('active')) {
        alarmaActuador.classList.remove('active');
        spanTexto.innerHTML = 'Alarma apagada';
        pTexto.innerHTML = 'Click para encender';
    } else {
        alarmaActuador.classList.add('active');
        spanTexto.innerHTML = 'Alarma activada';
        pTexto.innerHTML = 'Click para apagar';
    }
})


var options = {
    chart: {
        type: 'line',
        height: 250,
        toolbar: {
            show: false
        }
    },
    series: [{
        name: 'sales',
        data: [30,40,35,50,49,60,70,91,125]
    }],
    xaxis: {
        categories: [1991,1992,1993,1994,1995,1996,1997, 1998,1999]
    }
}
  
var chart = new ApexCharts((seccionGraficaAgua), options);
var chart2 = new ApexCharts((seccionGraficaPrecipitacion), options);
  
chart.render()
chart2.render()