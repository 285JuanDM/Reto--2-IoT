// Variables globales
let seccionGraficaAgua = document.querySelector('.grafica-nivel-agua');
let seccionGraficaPrecipitacion = document.querySelector('.grafica-nivel-precipitaciones');
let alarmaActuador = document.querySelector('.actuador-alarma');
let chart, chart2;
let websocket;

// Inicializar WebSocket
function initWebSocket() {
    console.log('Iniciando conexión WebSocket...');
    
    // Conectar al WebSocket en la misma dirección que la página web
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.host}/ws`;
    
    websocket = new WebSocket(wsUrl);
    
    websocket.onopen = function(evt) {
        console.log('WebSocket conectado');
    };
    
    websocket.onclose = function(evt) {
        console.log('WebSocket desconectado - reconectando...');
        setTimeout(initWebSocket, 2000);
    };
    
    websocket.onmessage = function(evt) {
        const datos = JSON.parse(evt.data);
        actualizarDatos(datos);
    };
    
    websocket.onerror = function(evt) {
        console.log('Error de WebSocket:', evt);
    };
}

// Evento de click para toggle de alarma
alarmaActuador.addEventListener('click', () => {
    let spanTexto = alarmaActuador.querySelector('span');
    let pTexto = alarmaActuador.querySelector('p');
    
    if (alarmaActuador.classList.contains('active')) {
        // Enviar comando para desactivar alarma
        if (websocket && websocket.readyState === WebSocket.OPEN) {
            websocket.send('alarma:off');
        }
    } else {
        // Enviar comando para activar alarma
        if (websocket && websocket.readyState === WebSocket.OPEN) {
            websocket.send('alarma:on');
        }
    }
});

// Inicializar las gráficas
function initCharts() {
    // Opciones para la gráfica de nivel de agua
    let optionsAgua = {
        chart: {
            type: 'line',
            height: 250,
            toolbar: {
                show: false
            },
            animations: {
                enabled: true,
                easing: 'easeinout',
                speed: 800
            }
        },
        title: {
            text: 'Nivel de agua',
            align: 'center'
        },
        colors: ['#0039D7'],
        series: [{
            name: 'Nivel (cm)',
            data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        }],
        xaxis: {
            categories: ['0h', '1h', '2h', '3h', '4h', '5h', '6h', '7h', '8h', '9h']
        },
        yaxis: {
            title: {
                text: 'Nivel (cm)'
            }
        },
        stroke: {
            curve: 'smooth',
            width: 3
        },
        markers: {
            size: 4
        }
    };
    
    // Opciones para la gráfica de precipitaciones
    let optionsPrecipitacion = {
        chart: {
            type: 'line',
            height: 250,
            toolbar: {
                show: false
            },
            animations: {
                enabled: true,
                easing: 'easeinout',
                speed: 800
            }
        },
        title: {
            text: 'Precipitaciones',
            align: 'center'
        },
        colors: ['#5725CD'],
        series: [{
            name: 'Nivel (mm)',
            data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        }],
        xaxis: {
            categories: ['0h', '1h', '2h', '3h', '4h', '5h', '6h', '7h', '8h', '9h']
        },
        yaxis: {
            title: {
                text: 'Nivel (mm)'
            }
        },
        stroke: {
            curve: 'smooth',
            width: 3
        },
        markers: {
            size: 4
        }
    };
    
    // Crear las gráficas
    chart = new ApexCharts(seccionGraficaAgua, optionsAgua);
    chart2 = new ApexCharts(seccionGraficaPrecipitacion, optionsPrecipitacion);
    
    chart.render();
    chart2.render();
}

// Actualizar los datos en la interfaz
function actualizarDatos(datos) {
    console.log('Datos recibidos:', datos);
    
    // Actualizar nivel de agua
    document.getElementById('valor-nivel-agua').textContent = datos.nivelAgua.toFixed(1) + ' cm';
    
    // Actualizar estado del agua y clases CSS
    const nivelAguaElement = document.querySelector('.alerta-nivel-agua .nivel-actual');
    const estadoAguaElement = document.querySelector('.alerta-nivel-agua .estado-actual');
    
    // Eliminar clases anteriores
    nivelAguaElement.classList.remove('normal', 'moderado', 'alto');
    estadoAguaElement.classList.remove('normal', 'moderado', 'alto');
    
    // Agregar clases según el estado
    nivelAguaElement.classList.add(datos.estadoAgua);
    estadoAguaElement.classList.add(datos.estadoAgua);
    
    // Actualizar texto del estado
    let textoEstadoAgua = 'Normal';
    if (datos.estadoAgua === 'moderado') textoEstadoAgua = 'Moderado';
    if (datos.estadoAgua === 'alto') textoEstadoAgua = '¡Alto!';
    document.getElementById('estado-nivel-agua').textContent = textoEstadoAgua;
    
    // Actualizar precipitaciones
    document.getElementById('valor-precipitaciones').textContent = datos.precipitaciones.toFixed(1) + ' mm';
    
    // Actualizar estado de precipitaciones y clases CSS
    const nivelPrecipitacionesElement = document.querySelector('.alerta-nivel-precipitaciones .nivel-actual');
    const estadoPrecipitacionesElement = document.querySelector('.alerta-nivel-precipitaciones .estado-actual');
    
    // Eliminar clases anteriores
    nivelPrecipitacionesElement.classList.remove('normal', 'moderado', 'alto');
    estadoPrecipitacionesElement.classList.remove('normal', 'moderado', 'alto');
    
    // Agregar clases según el estado
    nivelPrecipitacionesElement.classList.add(datos.estadoPrecipitaciones);
    estadoPrecipitacionesElement.classList.add(datos.estadoPrecipitaciones);
    
    // Actualizar texto del estado
    let textoEstadoPrecipitaciones = 'Normal';
    if (datos.estadoPrecipitaciones === 'moderado') textoEstadoPrecipitaciones = 'Moderada';
    if (datos.estadoPrecipitaciones === 'alto') textoEstadoPrecipitaciones = '¡Alta!';
    document.getElementById('estado-precipitaciones').textContent = textoEstadoPrecipitaciones;
    
    // Actualizar estado de la alarma
    if (datos.alarmaActivada) {
        alarmaActuador.classList.add('active');
        document.getElementById('texto-alarma').textContent = 'Alarma activada';
        document.getElementById('mensaje-alarma').textContent = 'Click para apagar';
    } else {
        alarmaActuador.classList.remove('active');
        document.getElementById('texto-alarma').textContent = 'Alarma apagada';
        document.getElementById('mensaje-alarma').textContent = 'Click para encender';
    }
    
    // Actualizar gráficas si hay datos históricos
    if (datos.aguaHistorico && datos.fechasHistorico) {
        chart.updateSeries([{
            name: 'Nivel (cm)',
            data: datos.aguaHistorico
        }]);
        
        chart.updateOptions({
            xaxis: {
                categories: datos.fechasHistorico
            }
        });
    }
    
    if (datos.precipitacionesHistorico && datos.fechasHistorico) {
        chart2.updateSeries([{
            name: 'Nivel (mm)',
            data: datos.precipitacionesHistorico
        }]);
        
        chart2.updateOptions({
            xaxis: {
                categories: datos.fechasHistorico
            }
        });
    }
}

// Actualizar hora y fecha
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

// Inicializar todo al cargar la página
document.addEventListener('DOMContentLoaded', () => {
    console.log('Página cargada - inicializando...');
    initCharts();
    initWebSocket();
    actualizarHoraFecha();
    setInterval(actualizarHoraFecha, 60000);
    
    // Alternativamente, si no hay WebSocket, intentar obtener datos por HTTP
    if (!WebSocket) {
        console.log('WebSocket no disponible - usando HTTP polling');
        // Fallback a polling HTTP
        setInterval(() => {
            fetch('/datos')
                .then(response => response.json())
                .then(data => actualizarDatos(data))
                .catch(error => console.error('Error fetching data:', error));
        }, 5000);
    }
});