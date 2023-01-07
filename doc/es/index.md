% Índice de Ayuda

# Introducción

Drumstick Metronome es un metrónomo basado en MIDI que utiliza el secuenciador ALSA.

La audiencia destinataria son músicos y estudiantes de música. Como cualquier 
metrónomo fisico es una herramienta para mantener el ritmo mientras se tocan 
instrumentos musicales.

Utiliza MIDI en lugar de audio digital, lo que permite un uso bajo de la CPU y temporización muy precisa
gracias al secuenciador ALSA. Esto significa que también necesita un sintetizador MIDI 
para la generación de sonido, porque el programa no produce ningún sonido
por sí mismo. Los mejores resultados provienen del uso de un sintetizador hardware, pero puede
utilizar también un sintetizador software en su lugar.

Solicite soporte, informe de problemas, errores y nuevas funciones en el sistema de seguimiento: 
[Sitio del proyecto SourceForge](https://sourceforge.net/p/kmetronome/support-requests/)

# Uso de Drumstick Metronome

## Características

Solo necesita ajustar algunos controles, como el control deslizante del tempo, el valor del
número de pulsos del compás y el selector de duración del pulso. Presione el botón de reproducción
para comenzar. Utilice el botón de parada a su conveniencia.

El tempo se puede configurar de 25 a 250 NPM usando el control deslizante. Las unidades son
negras por minuto (unidades de metrónomo Mälzel). También puede hacer doble clic
sobre la ventana principal para abrir un cuadro de diálogo donde puede ingresar un nuevo
tempo directamente con el teclado. También hay un cuadro combinado para elegir
y mostrar el tempo con nombres musicales italianos.

Partes/Compás se puede configurar de 1 a 32 beats. Estos son el número de pulsos
en cada compás, y es el numerador en el tipo de compás como sería anotado.

La figura rítmica es el denominador de la especificación del tipo de compás,
y representa la duración de cada pulso. Cambiar este valor no cambia el significado de 
las unidades de tempo.

Patrón es una lista desplegable para elegir una definición de patrón. El valor por defecto 
"Automático" significa que el programa genera patrones utilizando los sonidos 
establecidos en el cuadro de diálogo de configuración (Fuerte / Débil) y el ritmo
definido por "Partes/Compas" y "Figura rítmica". También contiene
los nombres de los patrones definidos por el usuario.

## Para Empezar

Este programa utiliza el protocolo MIDI, por lo que es una buena idea tener
algunas nociones básicas sobre MIDI para comprender completamente los conceptos
Detrás de eso. Aquí puede encontrar una buena introducción:
[Qué es MIDI](https://www.midi.org/midi-articles/categories/MIDI%201.0).

Drumstick Metronome produce eventos MIDI. Si quieres escuchar los eventos
traducido a sonidos, necesita conectar el puerto MIDI OUT desde este
programa al puerto MIDI IN de un sintetizador MIDI. Puede ser un
sintetizador MIDI hardware o software. Si es un sintetizador hardware externo,
también se necesita una interfaz MIDI compatible con ALSA
instalado en su computadora, y un cable MIDI conectado tanto a 
la interfaz MIDI del ordenador como a la toma MIDI IN del sintetizador.

Si no tiene un sintetizador MIDI externo, aún puede usar el
programa con un sintetizador software compatible MIDI con el secuenciador ALSA, como 
[QSynth](https://qsynth.sourceforge.io). Inicia tu
sintetizador software antes de Drumstick Metronome, y luego haz la conexión 
entre los dos programas, ya sea en el cuadro de diálogo de configuración de Drumstick Metronome 
o usando un administrador de conexión externo como [QJackCtl](https://qjackctl.sourceforge.io).

No olvide instalar una buena [Fuente de sonido](https://en.wikipedia.org/wiki/SoundFont) 
en el cuadro de diálogo "Configuración ..." de QSynth.

## Configuración

Drumstick Metronome tiene capacidades limitadas de gestión de sesiones. Puede recordar
una conexión para el puerto de salida ALSA, y una conexión para su
puerto de entrada. Las conexiones se almacenan cuando el programa termina y se recuerdan 
al inicio. No necesita esta función si prefiere hacer tales
conexiones a mano, utilizando un conector o cualquier otra utilidad equivalente, o
si usa un administrador de sesiones externo como el patchbay incluido en el
programa [QJackCtl](https://qjackctl.sourceforge.io).

Drumstick Metronome utiliza un archivo de definición de instrumento en formato .INS, el mismo
formato que Qtractor, TSE3, Cakewalk y Sonar. En el **instrumento receptor**, 
la lista desplegable permite elegir uno entre General MIDI, Roland GS y Yamaha XG. Puede agregar más definiciones creando
un archivo llamado `drums.ins` en `$HOME/.local/share/kmetronome.sourceforge.net`. Los
contenidos de las listas desplegables **Banco**, **Programa**, **Nota Débil** y **Nota fuerte** 
también dependen de esta definición.

**Canal** suele ser 10, es decir, el canal de percusión de un sintetizador General MIDI.
Debe ser un número entre 1 y 16.

**Resolución** es el número de tics (unidades de tiempo) para cada negra.
El valor varía de 48 a 960. El valor predeterminado es 120.

**Duración de la nota** es la duración (en número de tics) del lapso de tiempo
entre un evento NOTE ON y su correspondiente evento NOTE OFF. Este control está 
habilitado solo cuando **Emitir eventos NOTE OFF** también está habilitado. Valores muy bajos
puede causar clics silenciados en algunos sintetizadores.

Los sonidos de percusión generalmente no necesitan que se envíen eventos NOTE OFF
cada NOTE ON. Seleccione la casilla de verificación **Emitir eventos NOTE OFF** solo si su 
sintetizador o instrumento admite o requiere esta configuración.

**Banco** y **Programa** se utiliza para cambiar la batería de los instrumentos que 
soportan varios ajustes. Muchos sintetizadores no entienden el cambio de programa 
para el canal de percusión.

En el modo de patrón **Automático**, el sonido de **Nota fuerte** se reproduce como primer tiempo 
en cada compás, mientras que cualquier otro tiempo en el mismo compás se reproduce usando 
el sonido de **Nota débil**. Los valores numéricos 33 y 34 son los sonidos GM2 y XG
para el clic del metrónomo y la campana del metrónomo respectivamente.

## Editor de patrones

Con este cuadro de diálogo puede editar, probar y seleccionar patrones. Para crear 
nuevos patrones simplemente guarde la definición actual con un nuevo nombre.
Los patrones están representados por una tabla. Las filas de la tabla corresponden a
sonidos de percusión. Puede eliminar y agregar filas de una lista de sonidos 
definido por los ajustes del instrumento en el cuadro de diálogo de configuración. 
El número de columnas de la tabla determina la longitud del patrón, 
entre 1 y 99 elementos de cualquier duración de tiempo.

Cada celda de la tabla acepta valores entre N = 1 y 9, correspondientes a la 
velocidad MIDI (N*127/9) de las notas, o 0 para cancelar el sonido. Los valores 
válidos también son "f" (= fuerte) y ""p" (= piano) correspondientes a las 
velocidades variables definidas por los botones giratorios (Fuerte / Débil) en la ventana principal.
Los valores de las celdas se pueden seleccionar y modificar usando el teclado
o el ratón. No es necesario detener la reproducción antes de modificar los valores.

# Referencia de comandos

## La ventana principal

### El menú Archivo

**Archivo → Importar patrones**

: Importa definiciones de patrones en Drumstick Metronome

**Archivo → Exportar patrones**

: Exporta definiciones de patrones de Drumstick Metronome

**Archivo → Comenzar / Parar**

: Controla la reproducción del patrón

**Archivo → Salir**

: Cierra Drumstick Metronome

### El menú Editar

**Editar → Editar patrones**

: Abre el editor de patrones

### El menú de configuración

**Configuración → Mostrar botones de acción**

: Muestra u oculta los botones de acción. Hay elementos de menú equivalentes para cada botón.

**Configuración → Mostrar barra de herramientas**

: Muestra u oculta la barra de herramientas. Hay elementos de menú equivalentes para cada botón.

**Ajustes → Configuración**

: Configura Drumstick Metronome

### El menú de ayuda

**Ayuda → Contenido de la ayuda**

: Abre la ventana de ayuda

**Ayuda → Idioma**

: Abre un submenú que muestra las traducciones disponibles

**Ayuda → acerca de**

: Abre un cuadro acerca de este programa

**Ayuda → acerca de Qt**

: Abre un cuadro acerca de los marcos Qt

# Control externo

Drumstick Metronome se puede controlar mediante su interfaz D-Bus, MIDI System Realtime
y mensajes MIDI exclusivos del sistema.

## Interfaz D-Bus 

Drumstick Metronome proporciona algunas funciones a través de su interfaz D-Bus. Usted puede
use un programa habilitado para D-Bus para controlar Drumstick Metronome como `qdbusviewer` o el 
utilidad de línea de comandos `qdbus`.

Por ejemplo, estos comandos se pueden usar desde un intérprete de comandos:

    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.play
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.stop
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.cont
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTempo 150
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTimeSignature 3 8

## Mensajes exclusivos del sistema universal

Drumstick Metronome comprende algunos mensajes exclusivos del sistema universal. Porque
la ID del dispositivo aún no está implementada, todos los mensajes reconocidos deben
estar marcado como difusión (0x7F).

Mensaje de tiempo real: mensaje de cambio de medida del compás

    Formato: 0xF0 0x7F 0x7F 0x03 Comando Longitud Numerador Denominador ... 0xF7
                                 0x02: cambio de medida del compás 
                                 0x42: cambio de medida del compás diferida

Notas: ambos comandos afectan solo a los siguientes patrones programados después
recibiendo el comando. La longitud es al menos 2. Bytes adicionales siguientes
Se ignoran el numerador y el denominador.

Mensajes MMC

    Formato: 0xF0 0x7F 0x7F 0x06 Comando 0xF7
                                 0x01: Detener
                                 0x02: Reproducir
                                 0x03: Reproducción diferida

## Interfaz en tiempo real del sistema MIDI

Puede utilizar un dispositivo MIDI que genere mensajes MIDI System Realtime para
controlar Drumstick Metronome. Los mensajes del sistema en tiempo real aceptados son: Inicio (0xFA),
Deténgase (0xFC) y continúe (0xFB).

Muchos teclados MIDI maestros y dispositivos de botones MIDI proporcionan esos tres
Controles de transporte MIDI. Necesita conectar sus dispositivos MIDI externos
al ordenador mediante cables MIDI (o USB) y conecte el
correspondiente puerto ALSA al puerto de entrada de Drumstick Metronome. Esta conexión
debe hacerse a mano o con un software de gestión de sesiones como
QJackCtl.

# Créditos y Licencia

Programa Copyright © 2005-2023 Pedro Lopez-Cabanillas

Documentación Copyright © 2005-2023 Pedro Lopez-Cabanillas

Este programa es software gratuito; puedes redistribuirlo y / o modificarlo
bajo los términos de la Licencia Pública General GNU publicada por
la Free Software Foundation; ya sea la versión 2 de la licencia, o
(a su elección) cualquier versión posterior.

Este programa se distribuye con la esperanza de que sea de utilidad,
pero SIN NINGUNA GARANTÍA; sin siquiera la garantía implícita de
COMERCIABILIDAD o APTITUD PARA UN PROPÓSITO EN PARTICULAR. Ver el
Licencia pública general GNU para más detalles.
                                                                        
Debería haber recibido una copia de la Licencia Pública General GNU
junto con este programa. De lo contrario, consulte [https://www.gnu.org/licenses](https://www.gnu.org/licenses)

# Instalación
 
## Cómo obtener Drumstick Metronome

Aquí puede encontrar la última versión: 
[Archivos del proyecto](https://sourceforge.net/projects/kmetronome/files/kmetronome/)

## Requisitos

Para compilar y utilizar con éxito Drumstick Metronome, necesita Qt 5 o
posterior, Drumstick 2, drivers ALSA y librería ALSA.

También necesita un sintetizador MIDI para producir sonido.

El sistema de compilación requiere [CMake](https://cmake.org) 3.14 o más reciente.

La biblioteca, los controladores y las utilidades de ALSA se pueden encontrar en la
[Página de inicio de ALSA](https://www.alsa-project.org).

Drumstick::ALSA es un envoltorio C++ alrededor de la interfaz del secuenciador de ALSA usando
objetos, modismos y estilo Qt. Lo puedes encontrar en la 
[Página de inicio de Drumstick](https://drumstick.sourceforge.io).

Consulte también la [página de inicio de Drumstick Metronome](https://kmetronome.sourceforge.io/kmetronome.shtml).

## Compilación e instalación

Para compilar e instalar Drumstick Metronome en su sistema, escriba lo
siguiente en el directorio base de la distribución Drumstick Metronome:

    % cmake.
    % make
    % sudo make install

Dado que Drumstick Metronome usa `cmake` y `make`, no debería tener problemas
compilándolo. Si tiene algún problema, infórmelo al autor del proyecto en 
[sistema de seguimiento de errores](https://sourceforge.net/p/kmetronome/bugs/).
