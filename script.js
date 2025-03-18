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
    },
    title: {
        text: 'Nivel de agua',
        align: 'center'
    },
    colors: ['#0039D7'],
    series: [{
        name: 'sales',
        data: [30,40,35,50,49,60,70,91,125]
    }],
    xaxis: {
        categories: [1991,1992,1993,1994,1995,1996,1997, 1998,1999]
    }
}

var options2 = {
    chart: {
        type: 'line',
        height: 250,
    },
    title: {
        text: 'Precipitaciones',
        align: 'center'
    },
    colors: ['#0039D7'],
    series: [{
        name: 'sales',
        data: [30,40,35,50,49,60,70,91,125]
    }],
    xaxis: {
        categories: [1991,1992,1993,1994,1995,1996,1997, 1998,1999]
    }
}
  
var chart = new ApexCharts((seccionGraficaAgua), options);
var chart2 = new ApexCharts((seccionGraficaPrecipitacion), options2);
  
chart.render()
chart2.render()


function actualizarHoraFecha() {
    const ahora = new Date();
    
    let horas = ahora.getHours();
    const minutos = ahora.getMinutes();
    const ampm = horas >= 12 ? 'pm' : 'am';
    
    horas = horas % 12;
    horas = horas ? horas : 12;
    
    const minutosFormateados = minutos < 10 ? '0' + minutos : minutos;
    
    const horaFormateada = `${horas}:${minutosFormateados} ${ampm}`;
    
    const dia = ahora.getDate();
    const mes = ahora.getMonth();
    const año = ahora.getFullYear();
    
    const nombresMeses = ['Ene', 'Feb', 'Mar', 'Abr', 'May', 'Jun', 'Jul', 'Ago', 'Sep', 'Oct', 'Nov', 'Dic'];
    
    const fechaFormateada = `${dia} ${nombresMeses[mes]}, ${año}`;
    
    document.querySelector('.hora').textContent = horaFormateada;
    document.querySelector('.fecha').textContent = fechaFormateada;
}

actualizarHoraFecha();
setInterval(actualizarHoraFecha, 60000);