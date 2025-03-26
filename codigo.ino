#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include "time.h"
#include <vector>

const char *ssid = "Juanda";
const char *password = "del1al15.com";

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>HydroMeter</title>

        <style>
            @import url('https://fonts.googleapis.com/css2?family=Nunito:ital,wght@0,200..1000;1,200..1000&family=Playwrite+RO:wght@100..400&display=swap');
            
            *, *::before, *::after {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
            }

            body {
                background: #F9FAFE;
                font-family: 'Nunito', sans-serif;
            }

            html, body {
                height: 100%;
                margin: 0;
                display: flex;
                flex-direction: column;
            }

            button {
                all: unset;
            }

            .header-top {
                width: 100%;
                display: flex;
                justify-content: space-between;
                align-items: center;
                background: #fff;
                border-radius: 0 0 40px 40px;
                box-shadow: 0 1px 2px 0 rgba(0, 4, 255, 0.05);
                padding: 30px 50px;
            }

            .header-top .header-titulo {
                display: flex;
                align-items: center;
                gap: 10px;
                font-size: 30px;
                user-select: none;
                font-weight: 900;
                color: #0039D7;
            }

            .header-top .menu-botones {
                display: flex;
                align-items: center;
                gap: 20px;
            }

            .header-top .boton {
                width: 100px;
                padding: 10px 20px;
                border-radius: 10px;
                font-size: 17px;
                text-align: center;
                color: rgb(0, 57, 215);
                outline: 1px solid rgb(0, 57, 215, 0.5);
                background: rgb(0, 57, 215, 0.12);
                transition: ease-in-out all 0.2s;
                cursor: pointer;

                &:hover {
                    background: rgb(0, 57, 215, 0.2);
                    transition: ease-in-out all 0.2s;
                }
            }

            .header-top .boton.active {
                background: rgb(0, 57, 215, 0.2);
                transition: ease-in-out all 0.2s;
            }

            .header-top .hora-fecha {
                display: flex;
                justify-content: space-around;
                align-items: center;
                font-size: 17px;
                color: #0039D7;
            }

            .contenido-principal {
                flex: 1;
                display: grid;
                grid-template-columns: repeat(2, 1fr);
                grid-template-rows: auto 220px auto;
                gap: 10px;
                width: 100%;
                padding: 20px 5%;
                box-sizing: border-box;
            }

            .grafica-nivel-agua {
                grid-column: span 2;
            }

            .grafica-nivel-agua,
            .grafica-nivel-precipitaciones,
            .alerta-nivel-agua,
            .actuador-alarma,
            .alerta-nivel-precipitaciones,
            .toogle-alarma-automatica {
                width: 100%;
                height: 100%;
                border-radius: 20px;
                background: white;
                box-shadow: 0 2px 6px rgba(0, 4, 255, 0.05);
            }

            .toogle-alarma-automatica {
                display: flex;
                padding: 20px;
                align-items: center;
                justify-content: center;
                flex-direction: column;
                gap: 30px;
            }

            .toogle-alarma-automatica span {
                font-size: 40px;
                font-weight: 800;
                color: rgba(30, 27, 57, 0.8);
            }

            .theme-checkbox {
                --toggle-size: 20px;
                -webkit-appearance: none;
                -moz-appearance: none;
                appearance: none;
                width: 7.5em; /* Aumentado */
                height: 3.75em; /* Aumentado */
                background: -webkit-gradient(linear, left top, right top, color-stop(50%, #efefef), color-stop(50%, #2a2a2a)) no-repeat;
                background: -o-linear-gradient(left, #efefef 50%, #A01AA3 50%) no-repeat;
                background: linear-gradient(to right, #efefef 50%, #A01AA3 50%) no-repeat;
                background-size: 205%;
                background-position: 0;
                -webkit-transition: 0.4s;
                -o-transition: 0.4s;
                transition: 0.4s;
                border-radius: 99em;
                position: relative;
                cursor: pointer;
                font-size: var(--toggle-size);
            }
            
            .theme-checkbox::before {
                content: "";
                width: 2.75em; /* Aumentado */
                height: 2.75em; /* Aumentado */
                position: absolute;
                top: 0.5em;
                left: 0.5em;
                background: -webkit-gradient(linear, left top, right top, color-stop(50%, #efefef), color-stop(50%, #2a2a2a)) no-repeat;
                background: -o-linear-gradient(left, #efefef 50%, #4b4b4b 50%) no-repeat;
                background: linear-gradient(to right, #efefef 50%, #4b4b4b 50%) no-repeat;
                background-size: 205%;
                background-position: 100%;
                border-radius: 50%;
                -webkit-transition: 0.4s;
                -o-transition: 0.4s;
                transition: 0.4s;
            }
            
            .theme-checkbox:checked::before {
                left: calc(100% - 2.75em - 0.5em); /* Ajustado al nuevo tamaño */
                background-position: 0;
            }
            
            .theme-checkbox:checked {
                background-position: 100%;
            }

            .toogle-button {
                display: flex;
                align-items: center;
                justify-content: center;
                flex-direction: column;
                gap: 10px;
                font-size: 20px;
                color: rgba(30, 27, 57, 0.8);
            }

            .grafica-nivel-agua, .grafica-nivel-precipitaciones {
                display: flex;
                padding: 0 20px;
                align-items: center;
                justify-content: center;
                flex-direction: column;
            }

            .actuador-alarma {
                display: flex;
                align-items: center;
                justify-content: center;
                flex-direction: column;
                text-align: center;
                background: #EFEFEF;
                transition: ease-in-out all 0.2s;
                outline: 1px solid rgba(0,0, 0, 0.2);
                cursor: pointer;

                &:hover {
                    background: #dfdddd;
                    transition: ease-in-out all 0.2s;
                }
            }

            .actuador-alarma .titulo-alarma {
                display: flex;
                align-items: center;
                gap: 10px;
                color: rgba(30, 27, 57, 0.7);
            }

            .actuador-alarma span {
                font-size: 40px;
                font-weight: 800;
            }

            .actuador-alarma p {
                font-size: 20px;
                font-weight: 250;
                color: rgba(30, 27, 57, 0.7);
            }

            .alerta-nivel-agua {
                display: flex;
                padding: 10px;
            }

            .alerta-nivel-agua .nivel-actual {
                flex: 1.3;
                display: flex;
                align-items: center;
                justify-content: center;
                flex-direction: column;
                border-radius: 20px;
            }

            .alerta-nivel-agua .nivel-actual p {
                font-size: 15px;
                font-weight: 250;
                color: #fff;
            }

            .alerta-nivel-agua .nivel-actual span {
                font-size: 40px;
                font-weight: 800;
                color: #fff;
            }

            .alerta-nivel-agua .estado-actual {
                flex: 3;
                display: flex;
                align-items: center;
                justify-content: center;
                flex-direction: column;
            }

            .alerta-nivel-agua .estado-actual p {
                font-size: 15px;
                font-weight: 250;
            }

            .alerta-nivel-agua .estado-actual span {
                font-size: 40px;
                font-weight: 800;
            }

            .alerta-nivel-precipitaciones {
                display: flex;
                padding: 10px;
            }

            .alerta-nivel-precipitaciones .nivel-actual {
                flex: 1.3;
                display: flex;
                align-items: center;
                justify-content: center;
                flex-direction: column;
                border-radius: 20px;
            }

            .alerta-nivel-precipitaciones .nivel-actual p {
                font-size: 15px;
                font-weight: 250;
                color: #fff;
            }

            .alerta-nivel-precipitaciones .nivel-actual span {
                font-size: 40px;
                font-weight: 800;
                color: #fff;
            }

            .alerta-nivel-precipitaciones .estado-actual {
                flex: 3;
                display: flex;
                align-items: center;
                justify-content: center;
                flex-direction: column;
            }

            .alerta-nivel-precipitaciones .estado-actual p {
                font-size: 15px;
                font-weight: 250;
            }

            .alerta-nivel-precipitaciones .estado-actual span {
                font-size: 40px;
                font-weight: 800;
            }

            .actuador-alarma.active {
                background: #A01AA3;
                transition: ease-in-out all 0.2s;

                &:hover {
                    background: #931895;
                }
            }

            .actuador-alarma.active .titulo-alarma {
                color: #fff;
            }

            .actuador-alarma.active p {
                color: #fff;
            }

            .actuador-alarma.active svg path {
                fill: #fff;
                fill-opacity: 100%;
            }

            .nivel-actual.normal, .nivel-actual.no-lluvia {
                background: #0039D7;
            }

            .estado-actual.normal, .estado-actual.no-lluvia {
                color: #0039D7;
            }

            .nivel-actual.moderado {
                background: #5725CD;
            }

            .estado-actual.moderado {
                color: #5725CD;
            }

            .nivel-actual.alto, .nivel-actual.lluvia {
                background: #A01AA3;
            }

            .estado-actual.alto, .estado-actual.lluvia {
                color: #A01AA3;
            }

            #checkbox {
                display: none;
            }

            .switch {
                position: relative;
                width: 70px;
                height: 70px;
                background-color: rgb(99, 99, 99);
                border-radius: 50%;
                z-index: 1;
                cursor: pointer;
                display: flex;
                align-items: center;
                justify-content: center;
                border: 2px solid rgb(126, 126, 126);
                box-shadow: 0px 0px 3px rgb(2, 2, 2) inset;
            }

            .switch svg {
                width: 1.2em;
            }

            .switch svg path {
                fill: rgb(48, 48, 48);
            }

            #checkbox:checked + .switch {
                box-shadow: 0px 0px 1px rgb(151, 243, 255) inset,
                    0px 0px 2px rgb(151, 243, 255) inset, 0px 0px 10px rgb(151, 243, 255) inset,
                    0px 0px 40px rgb(151, 243, 255), 0px 0px 100px rgb(151, 243, 255),
                    0px 0px 5px rgb(151, 243, 255);
                border: 2px solid rgb(255, 255, 255);
                background-color: rgb(146, 180, 184);
            }

            #checkbox:checked + .switch svg {
                filter: drop-shadow(0px 0px 5px rgb(151, 243, 255));
            }

            #checkbox:checked + .switch svg path {
                fill: rgb(255, 255, 255);
            }

            .historico-datos {
                display: flex;
                justify-content: center;
                align-items: flex-start;
                width: 100%;
                height: 100vh; /* Ocupar toda la pantalla */
                padding: 20px;
                margin-top: 20px; /* Separación del borde superior */
                overflow: hidden; /* Evita que el contenedor haga scroll */
            }

            #tablaHistorico {
                width: 100%;
                border-collapse: collapse;
                text-align: center;
                box-shadow: 0 0 20px rgba(0, 0, 0, 0.15);
                border-radius: 8px;
                overflow: hidden;
                display: table;
                table-layout: fixed;
            }

            #tablaHistorico thead {
                position: sticky;
                top: 0;
                background-color: #0039D7;
                color: white;
                font-weight: bold;
                z-index: 2;
            }

            #tablaHistorico th,
            #tablaHistorico td {
                padding: 12px 15px;
                border-bottom: 1px solid #dddddd;
                white-space: nowrap; /* Evita el salto de línea en columnas */
            }

            #tablaHistorico tbody {
                display: block;
                overflow-y: auto;
                height: calc(100vh - 150px); /* Altura ajustada considerando la cabecera */
            }

            #tablaHistorico thead, #tablaHistorico tbody tr {
                display: table;
                width: 100%;
                table-layout: fixed;
            }

            #tablaHistorico tbody::-webkit-scrollbar {
                display: none; /* Para Chrome, Safari y Edge */
            }

            #tablaHistorico tbody tr {
                display: table;
                width: 100%;
                table-layout: fixed;
            }

            #tablaHistorico tbody tr:nth-of-type(even) {
                background-color: #f3f3f3;
            }

            #tablaHistorico tbody tr:hover {
                background-color: #e0e0ff;
                cursor: pointer;
            }

        </style>
    </head>

    <body>
        <header class="header-top">
            <div class="header-titulo">
                <svg width="34" height="34" viewBox="0 0 40 40" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M20 35.8333C16.3056 35.8333 13.1597 34.5556 10.5625
                            32C7.96528 29.4444 6.66667 26.3333 6.66667 22.6667C6.66667
                            20.9167 7.00695 19.2431 7.6875 17.6458C8.36806 16.0486 9.33333
                            14.6389 10.5833 13.4167L20 4.16667L29.4167 13.4167C30.6667 14.6389
                            31.6319 16.0486 32.3125 17.6458C32.9931 19.2431 33.3333 20.9167
                            33.3333 22.6667C33.3333 26.3333 32.0347 29.4444 29.4375 32C26.8403
                            34.5556 23.6944 35.8333 20 35.8333Z"
                    fill="#0039D7"/>
                </svg>

                <span>HydroMeter</span>
            </div>

            <div class="menu-botones">
                <button class="boton dashboard active">Dashboard</button>
                <button class="boton historico">Historico</button>
            </div>

            <div class="hora-fecha">
                <p>
                    <span class="fecha">16 Jul, 2025</span>
                    <span class="separador">•</span>
                    <span class="hora">12:00 pm</span>
                </p>
            </div>
        </header>

        <main class="contenido-principal">
            <section class="grafica-nivel-agua"></section>

            <section class="alerta-nivel-agua">
                <div class="nivel-actual normal">
                    <p>Último valor:</p>
                    <span id="valor-nivel-agua">0 cm</span>
                    <p>Tomado hace unos segundos</p>
                </div>
                
                <div class="estado-actual normal">
                    <p>Estado del nivel del agua:</p>
                    <span id="estado-nivel-agua">Normal</span>
                    <p class="niveles-info">Se considera alto de 10 cm en adelante</p>
                </div>
            </section>

            <section class="alerta-nivel-precipitaciones">
                <div class="estado-actual no-lluvia">
                    <p>Detección de precipitaciones:</p>
                    <span id="estado-precipitaciones">¿Esta lloviendo?</span>
                </div>

                <div class="nivel-actual no-lluvia">
                    <p>Último valor:</p>
                    <span id="valor-precipitaciones">Sí</span>
                    <p>Tomado hace unos segundos</p>
                </div>
            </section>

            <section class="actuador-alarma">
                <div class="titulo-alarma">
                    <span id="texto-alarma">Alarma apagada</span>
                    <svg
                        width="40"
                        height="40"
                        viewBox="0 0 76 76"
                        fill="none"
                        xmlns="http://www.w3.org/2000/svg">

                        <path d="M12.6667 60.1667H63.3334V53.8333H12.6667V60.1667ZM25.3334
                                41.1667H31.6667V31.6667C31.6667 29.925 32.2868 28.434 33.5271
                                27.1938C34.7674 25.9535 36.2584 25.3333 38 25.3333V19C34.5167
                                19 31.5348 20.2403 29.0542 22.7208C26.5737 25.2014 25.3334 28.1833
                                25.3334 31.6667V41.1667ZM22.1667 47.5H53.8334V31.6667C53.8334 27.2861
                                52.2896 23.5521 49.2021 20.4646C46.1146 17.3771 42.3806 15.8333 38
                                15.8333C33.6195 15.8333 29.8855 17.3771 26.798 20.4646C23.7105 23.5521
                                22.1667 27.2861 22.1667 31.6667V47.5ZM12.6667 66.5C10.925 66.5 9.43407
                                65.8799 8.19379 64.6396C6.95351 63.3993 6.33337 61.9083 6.33337
                                60.1667V53.8333C6.33337 52.0917 6.95351 50.6007 8.19379 49.3604C9.43407
                                48.1201 10.925 47.5 12.6667 47.5H15.8334V31.6667C15.8334 25.4917 17.9841
                                20.2535 22.2855 15.9521C26.5868 11.6507 31.825 9.5 38 9.5C44.175 9.5 49.4132
                                11.6507 53.7146 15.9521C58.016 20.2535 60.1667 25.4917 60.1667 31.6667V47.5H63.3334C65.075
                                47.5 66.566 48.1201 67.8063 49.3604C69.0466 50.6007 69.6667 52.0917 69.6667 53.8333V60.1667C69.6667
                                61.9083 69.0466 63.3993 67.8063 64.6396C66.566 65.8799 65.075 66.5 63.3334 66.5H12.6667Z" 
                                fill="#1E1B39"
                                fill-opacity="0.7"/>
                        
                    </svg>
                </div>

                <p class="mensaje-alarma" id="mensaje-alarma">Click para encender</p>
            </section>
            
            <section class="toogle-alarma-automatica">
                <span>Alarma automática</span>
                
                <div class="toogle-button">
                    <input type="checkbox" class="theme-checkbox">
                    <p>Manual</p>
                </div>
            </section>
        </main>

        <main class="historico-datos">
            <table id="tablaHistorico" class="display">
                <thead>
                    <tr>
                        <th>ID</th>
                        <th>Fecha</th>
                        <th>Hora</th>
                        <th>Nivel de agua (cm)</th>
                        <th>Estado del nivel del agua</th>
                        <th>¿Llovía?</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>1</td>
                        <td>22/03/2025</td>
                        <td>10:30</td>
                        <td>150</td>
                        <td>Alerta</td>
                        <td>Sí</td>
                    </tr>
                    <tr>
                        <td>2</td>
                        <td>21/03/2025</td>
                        <td>14:15</td>
                        <td>90</td>
                        <td>Normal</td>
                        <td>No</td>
                    </tr>
                    <!-- Puedes agregar más registros dinámicamente -->
                </tbody>
            </table>
        </main>

        <script src="https://cdn.jsdelivr.net/npm/apexcharts"></script>
        <script>
            const socket = new WebSocket('ws://' + window.location.host + '/ws');
  
            // Variables para almacenar las referencias a los elementos del DOM
            const valorNivelAgua = document.getElementById("valor-nivel-agua");
            const estadoNivelAgua = document.getElementById("estado-nivel-agua");
            const valorPrecipitaciones = document.getElementById("valor-precipitaciones");
            const estadoPrecipitaciones = document.getElementById("estado-precipitaciones");
            const actuadorAlarma = document.querySelector(".actuador-alarma");
            const toogleButonAlarma = document.querySelector(".theme-checkbox");
            const toogleTextAlarma = document.querySelector(".toogle-button p");
            const textoAlarma = document.getElementById("texto-alarma");
            const mensajeAlarma = document.getElementById("mensaje-alarma");
            const nivelActualAgua = document.querySelector(".alerta-nivel-agua .nivel-actual");
            const estadoActualAgua = document.querySelector(".alerta-nivel-agua .estado-actual");
            const nivelActualAguap = document.querySelector(".alerta-nivel-agua .estado-actual .niveles-info");
            const nivelActualPrecipitacionesp = document.querySelector(".alerta-nivel-precipitaciones .estado-actual .niveles-info");
            const nivelActualPrecipitaciones = document.querySelector(".alerta-nivel-precipitaciones .nivel-actual");
            const estadoActualPrecipitaciones = document.querySelector(".alerta-nivel-precipitaciones .estado-actual");
            const mainHistorico = document.querySelector(".historico-datos");
            const mainDashboard = document.querySelector(".contenido-principal");
            const botonDashboard = document.querySelector(".boton.dashboard");
            const botonHistorico = document.querySelector(".boton.historico");
            const tablaHistorico = document.getElementById("tablaHistorico");

            mainHistorico.style.display = "none";

            botonDashboard.addEventListener("click", () => {
                botonDashboard.classList.add("active");
                botonHistorico.classList.remove("active");
                mainDashboard.style.display = "grid";
                mainHistorico.style.display = "none";
            });

            botonHistorico.addEventListener("click", () => {
                botonHistorico.classList.add("active");
                botonDashboard.classList.remove("active");
                mainDashboard.style.display = "none";
                mainHistorico.style.display = "grid";
            });
            
            // Configuración de las gráficas
            const optionsAgua = {
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
                  categories: []
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
  
            const graficaAgua = new ApexCharts(document.querySelector(".grafica-nivel-agua"), optionsAgua);
            graficaAgua.render();
  
            // Manejo de mensajes entrantes
            socket.onmessage = function (event) {
                const data = JSON.parse(event.data);
  
                console.log("Datos recibidos:", data);
                
                valorNivelAgua.textContent = data.nivelAgua + " cm";
                estadoNivelAgua.textContent = data.estadoAgua.charAt(0).toUpperCase() + data.estadoAgua.slice(1);
                
                valorPrecipitaciones.textContent = data.estadoPrecipitaciones == "lluvia" ? "Sí" : "No";
                
                actuadorAlarma.classList.toggle("active", data.alarmaActivada);
                textoAlarma.textContent = data.alarmaActivada ? "Alarma encendida" : "Alarma apagada";
                mensajeAlarma.textContent = data.alarmaActivada ? "Click para apagar" : "Click para encender";
                
                // Actualización de gráficas
                graficaAgua.updateSeries([{ data: data.aguaHistorico.slice(-10) }]);

                // Extraer las fechas y horas correctamente
                const horas = data.fechasHistorico.slice(-10).map(f => f[1]); // Obtiene las ultimas 10 horas (segundo elemento)
                const ultimaFecha = data.fechasHistorico[data.fechasHistorico.length - 1][0]; // Última fecha registrada (primer elemento)

                // Actualizar datos de la gráfica de Nivel de Agua
                graficaAgua.updateOptions({
                    title: {
                        text: `Nivel de agua - ${ultimaFecha}`,
                        align: 'center'
                    },
                    xaxis: {
                        categories: horas // Todas las horas como etiquetas del eje X
                    }
                });
  
                // Actualización de clases de estado
                nivelActualAgua.className = `nivel-actual ${data.estadoAgua}`;
                estadoActualAgua.className = `estado-actual ${data.estadoAgua}`;

                if (data.estadoAgua == "normal") {
                    nivelActualAguap.textContent = "Se considera normal de 0 cm ~ 6 cm";
                } else if (data.estadoAgua == "moderado") {
                    nivelActualAguap.textContent = "Se considera moderado de 6 cm ~ 10 cm";
                } else if (data.estadoAgua == "alto") {
                    nivelActualAguap.textContent = "Se considera alto de 10 cm en adelante";
                }
                
                nivelActualPrecipitaciones.className = `nivel-actual ${data.estadoPrecipitaciones}`;
                estadoActualPrecipitaciones.className = `estado-actual ${data.estadoPrecipitaciones}`;

                if (data.alarmaAutomatica) {
                    toogleButonAlarma.checked = true;
                    // Solo actualiza el texto si no está en medio de una interacción del usuario
                    if (!toogleButonAlarma.dataset.userChanging) {
                        toogleTextAlarma.textContent = "Automatica";
                        mensajeAlarma.textContent = "Modo automático";
                    }
                } else {
                    toogleButonAlarma.checked = false;
                    // Solo actualiza el texto si no está en medio de una interacción del usuario
                    if (!toogleButonAlarma.dataset.userChanging) {
                        toogleTextAlarma.textContent = "Manual";
                    }
                }

                // Actualización de la tabla de historial
                const tbody = tablaHistorico.querySelector("tbody");
                tbody.innerHTML = ""; // Limpiar contenido actual

                // Verificar si hay datos históricos en la respuesta
                if (data.datosHistoricos && Array.isArray(data.datosHistoricos)) {
                    // Iterar a través de los datos históricos
                    data.datosHistoricos.forEach((registro, index) => {
                        const fila = document.createElement("tr");
                        
                        // Crear las celdas con los datos
                        // Asumo que cada registro tiene estos campos, ajusta según tu estructura de datos real
                        const id = document.createElement("td");
                        id.textContent = index + 1;
                        
                        const fecha = document.createElement("td");
                        fecha.textContent = registro.fecha || data.fechasHistorico[index][0];
                        
                        const hora = document.createElement("td");
                        hora.textContent = registro.hora || data.fechasHistorico[index][1];
                        
                        const nivelAgua = document.createElement("td");
                        nivelAgua.textContent = (registro.nivelAgua || data.aguaHistorico[index]) + " cm";
                        
                        const estadoAgua = document.createElement("td");
                        let estadoTexto = "";
                        let nivelAguaValue = registro.nivelAgua || data.aguaHistorico[index];
                        
                        // Determinar el estado según el nivel del agua
                        if (nivelAguaValue <= 6) {
                            estadoTexto = "Normal";
                        } else if (nivelAguaValue <= 10) {
                            estadoTexto = "Moderado";
                        } else {
                            estadoTexto = "Alto";
                        }
                        estadoAgua.textContent = estadoTexto;
                        
                        const lluvia = document.createElement("td");
                        // Determinar si llovía según el nivel de precipitaciones
                        let precipitacionValue = registro.precipitaciones || data.precipitacionesHistorico[index];
                        let lluviaTexto = precipitacionValue == "lluvia" ? "Sí" : "No";
                        lluvia.textContent = lluviaTexto;
                        
                        // Añadir todas las celdas a la fila
                        fila.appendChild(id);
                        fila.appendChild(fecha);
                        fila.appendChild(hora);
                        fila.appendChild(nivelAgua);
                        fila.appendChild(estadoAgua);
                        fila.appendChild(lluvia);
                        
                        // Añadir la fila a la tabla
                        tbody.appendChild(fila);
                    });
                } else {
                    // Si no hay datos históricos, crear filas con los datos actuales de las gráficas
                    for (let i = 0; i < data.aguaHistorico.length; i++) {
                        const fila = document.createElement("tr");
                        
                        const id = document.createElement("td");
                        id.textContent = i + 1;
                        
                        const fecha = document.createElement("td");
                        fecha.textContent = data.fechasHistorico[i] ? data.fechasHistorico[i][0] : ultimaFecha;
                        
                        const hora = document.createElement("td");
                        hora.textContent = data.fechasHistorico[i] ? data.fechasHistorico[i][1] : "-";
                        
                        const nivelAgua = document.createElement("td");
                        nivelAgua.textContent = data.aguaHistorico[i] + " cm";
                        
                        const estadoAgua = document.createElement("td");
                        let estadoTexto = "";
                        let nivelAguaValue = data.aguaHistorico[i];
                        
                        // Determinar el estado según el nivel del agua
                        if (nivelAguaValue <= 6) {
                            estadoTexto = "Normal";
                        } else if (nivelAguaValue <= 10) {
                            estadoTexto = "Moderado";
                        } else {
                            estadoTexto = "Alto";
                        }
                        estadoAgua.textContent = estadoTexto;
                        
                        const lluvia = document.createElement("td");
                        
                        // Determinar si llovía según el nivel de precipitaciones
                        let precipitacionValue = data.precipitacionesHistorico[i];
                        let lluviaTexto = precipitacionValue > 0 ? "Sí" : "No";
                        lluvia.textContent = lluviaTexto;
                        
                        // Añadir todas las celdas a la fila
                        fila.appendChild(id);
                        fila.appendChild(fecha);
                        fila.appendChild(hora);
                        fila.appendChild(nivelAgua);
                        fila.appendChild(estadoAgua);
                        fila.appendChild(lluvia);
                        
                        // Añadir la fila a la tabla
                        tbody.appendChild(fila);
                    }
                }

            };
  
            // Evento para activar/desactivar la alarma
            actuadorAlarma.addEventListener("click", () => {
                const estado = actuadorAlarma.classList.contains("active") ? "off" : "on";
                socket.send(`alarma:${estado}`);
            });
  
            // Evento para cambiar el modo de la alarma
            toogleButonAlarma.addEventListener("change", () => {
                const esAutomatico = toogleButonAlarma.checked;
                
                // Actualiza la UI inmediatamente sin esperar respuesta
                toogleTextAlarma.textContent = esAutomatico ? "Automatica" : "Manual";
                
                if (esAutomatico) {
                    mensajeAlarma.textContent = "Modo automático";
                }
                
                // Indica que está en proceso de cambio por el usuario
                toogleButonAlarma.dataset.userChanging = "true";
                
                // Envía el comando al servidor
                const estado = esAutomatico ? "automatica" : "manual";
                socket.send(`alarmaAutomatica:${estado}`);
                
                // Elimina la marca de cambio por usuario después de un breve tiempo
                setTimeout(() => {
                    delete toogleButonAlarma.dataset.userChanging;
                }, 1000); // 1 segundo debería ser suficiente para evitar que la respuesta del servidor sobrescriba
            });
        </script>
    </body>
</html>
)rawliteral";

#define TRIG_PIN  5  // Pin de trigger del sensor ultrasónico
#define ECHO_PIN  18 // Pin de eco del sensor ultrasónico
#define RAIN_SENSOR_PIN  4  // Sensor de lluvia (entrada digital)
#define BUZZER_PIN  19  // Buzzer (salida)

float nivelAgua = 5.0;
float precipitaciones = 3.0;
bool alarmaActivada = false;
bool alarmaAutomatica = false;

const float UMBRAL_AGUA_MODERADO = 6.0;
const float UMBRAL_AGUA_ALTO = 10.0;
const float UMBRAL_PRECIPITACIONES_MODERADO = 4.0;
const float UMBRAL_PRECIPITACIONES_ALTO = 6.0;
const int ALTURA_MAQUETA = 28;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

std::vector<float> historicoAgua;
std::vector<float> historicoPrecipitaciones;
std::vector<std::pair<String, String>> tiempoHistorico;  // Guardar fecha y hora

// Configuración del servidor NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000; // UTC-5 para Colombia
const int   daylightOffset_sec = 0;

void medirSensores(void *parameter);
void handleWebSocketMessageActuador(void *arg, uint8_t *data, size_t len);
void handleWebSocketMessageAlarmaAutomatica(uint8_t num, uint8_t *payload, size_t length);
void notifyClients();
String* obtenerHoraYFechaActual();
String getEstadoNivelAgua();
String getEstadoNivelPrecipitaciones();
String generarJSON();

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.print("Conectado, IP: ");
  Serial.println(WiFi.localIP());

  // Configurar la obtención de hora con NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", html_page);
  });

  server.on("/datos", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = generarJSON();
    request->send(200, "application/json", json);
  });

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
      Serial.println("Cliente WebSocket conectado.");
    } else if (type == WS_EVT_DATA) {
      handleWebSocketMessageAlarmaAutomatica(client->id(), data, len);  // Pasamos el ID del cliente como uint8_t

      if (!alarmaAutomatica) {
        handleWebSocketMessageActuador(arg, data, len);
      }
    }
  });
  server.addHandler(&ws);
  server.begin();

  xTaskCreatePinnedToCore(
    medirSensores,
    "MedirSensores",
    4096,
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {
  ws.cleanupClients();
}

void medirSensores(void *parameter) {
  while (true) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = (duration * 0.034) / 2;

    nivelAgua = ALTURA_MAQUETA - floor(distance);
    precipitaciones = digitalRead(RAIN_SENSOR_PIN);

    Serial.printf("Nivel Agua: %.2f cm, Precipitaciones: %.2f \n", nivelAgua, precipitaciones);
    
    if (alarmaAutomatica && nivelAgua >= UMBRAL_AGUA_ALTO) {
      alarmaActivada = true;
    } else if (alarmaAutomatica && nivelAgua < UMBRAL_AGUA_ALTO) {
      alarmaActivada = false;
    }

    digitalWrite(BUZZER_PIN, alarmaActivada ? HIGH : LOW);

    if (historicoAgua.size() >= 2000) {  // Mantener solo los últimos 100 registros
        historicoAgua.erase(historicoAgua.begin());
        historicoPrecipitaciones.erase(historicoPrecipitaciones.begin());
        tiempoHistorico.erase(tiempoHistorico.begin());
    }

    // Agregar nuevos valores
    historicoAgua.push_back(nivelAgua);
    historicoPrecipitaciones.push_back(precipitaciones);
    String* horaFecha = obtenerHoraYFechaActual();
    tiempoHistorico.push_back({horaFecha[0], horaFecha[1]});

    String json = generarJSON();
    ws.textAll(json);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void handleWebSocketMessageActuador(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String mensaje = (char *)data;

    if (mensaje == "alarma:on") {
      alarmaActivada = true;
      Serial.println("Alarma activada desde el cliente web");
    } else if (mensaje == "alarma:off") {
      alarmaActivada = false;
      Serial.println("Alarma desactivada desde el cliente web");
    }
    notifyClients();
  }
}

void handleWebSocketMessageAlarmaAutomatica(uint8_t num, uint8_t *payload, size_t length) {
    String message = String((char *)payload).substring(0, length);
    
    if (message == "alarmaAutomatica:automatica") {
        alarmaAutomatica = true;
    } else if (message == "alarmaAutomatica:manual") {
        alarmaAutomatica = false;
    }
    Serial.print("Alarma Automática: ");
    Serial.println(alarmaAutomatica ? "Activada" : "Desactivada");
}

void notifyClients() {
  String json = generarJSON();
  ws.textAll(json);
}

String generarJSON() {
  DynamicJsonDocument doc(512);
  
  doc["nivelAgua"] = nivelAgua;
  doc["estadoAgua"] = getEstadoNivelAgua();
  doc["precipitaciones"] = precipitaciones;
  doc["estadoPrecipitaciones"] = getEstadoNivelPrecipitaciones();
  doc["alarmaActivada"] = alarmaActivada;
  doc["alarmaAutomatica"] = alarmaAutomatica;

  JsonArray aguaHistorico = doc.createNestedArray("aguaHistorico");
  JsonArray precipitacionesHistorico = doc.createNestedArray("precipitacionesHistorico");
  JsonArray fechasHistorico = doc.createNestedArray("fechasHistorico");

  for (size_t i = 0; i < historicoAgua.size(); i++) {
    aguaHistorico.add(historicoAgua[i]);
    precipitacionesHistorico.add(historicoPrecipitaciones[i]);
    
    JsonArray fechaArray = fechasHistorico.createNestedArray();
    fechaArray.add(tiempoHistorico[i].first);  // Hora
    fechaArray.add(tiempoHistorico[i].second); // Fecha
  }

  String json;
  serializeJson(doc, json);
  return json;
}

String* obtenerHoraYFechaActual() {
  static String resultado[2];
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error obteniendo la hora");
    resultado[0] = "2000-01-01"; // Fecha en formato YYYY-MM-DD
    resultado[1] = "00:00:00";   // Hora
    return resultado;
  }

  char fechaBuffer[11];  // YYYY-MM-DD + null terminator
  char horaBuffer[9];    // HH:MM:SS + null terminator

  strftime(fechaBuffer, sizeof(fechaBuffer), "%Y-%m-%d", &timeinfo);
  strftime(horaBuffer, sizeof(horaBuffer), "%H:%M:%S", &timeinfo);

  resultado[0] = String(fechaBuffer);  // Fecha primero
  resultado[1] = String(horaBuffer);   // Hora después
  return resultado;
}

String getEstadoNivelAgua() {
  if (nivelAgua >= UMBRAL_AGUA_ALTO) return "alto";
  if (nivelAgua >= UMBRAL_AGUA_MODERADO) return "moderado";
  return "normal";
}

String getEstadoNivelPrecipitaciones() {
  if (precipitaciones == 0) return "lluvia";
  if (precipitaciones == 1) return "no-lluvia";
}
