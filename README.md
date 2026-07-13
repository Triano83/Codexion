*Este proyecto ha sido creado como parte del currículo de 42 por jotriano.*

# Codexion

## Descripción

Codexion es un proyecto de manejo de hilos en C que simula un entorno de co-working inclusivo. El objetivo es coordinar a varias personas que programan, representadas por hilos, y que deben compartir recursos limitados para poder compilar, depurar y refactorizar su código.

El sistema incorpora un árbitro (que es otro hilo) con un planificador basado en una cola de prioridad tipo Min-Heap, capaz de trabajar con dos políticas de asignación:

- **FIFO (First In, First Out)**
- **EDF (Earliest Deadline First)**

El reto principal es garantizar que todas las personas avancen sin interbloqueos, ni condiciones de carrera, respetando además los tiempos de uso y enfriamiento de los recursos.

## Compilación

Para compilar el proyecto utiliza el `Makefile` incluido:

- `make` o `make all`: compila el ejecutable `codexion`.
- `make clean`: elimina la carpeta `/obj` y los archivos objeto `.o`.
- `make fclean`: elimina la carpeta `/obj`, los archivos objeto `.o` y el ejecutable.
- `make re`: recompila todo desde cero.

## Ejecución

La simulación se ejecuta con el siguiente formato:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Todos los argumentos son obligatorios y deben ser enteros positivos, excepto `scheduler`, que puede ser `fifo` o `edf`.

### Ejemplo

```bash
./codexion 3 1200 200 200 200 5 10 edf
```

## Recursos

- Documentación oficial de POSIX Threads (`pthreads`).
- *Operating System Concepts* de Silberschatz para comprender problemas clásicos de concurrencia.
- Uso de IA para apoyo en la explicación del Min-Heap y busqueda de informacion de Hilos.

## Gestión de bloqueos

La solución implementa mecanismos para evitar los problemas de concurrencia más habituales:

- **Prevención de interbloqueos**: se utiliza un árbitro central que controla la asignación de recursos y evita esperas circulares.
- **Prevención de inanición**: el planificador Min-Heap prioriza solicitudes según la política elegida, permitiendo una asignación ordenada y justa.
- **Gestión del cooldown**: cuando un recurso se libera, entra en un periodo de enfriamiento antes de volver a estar disponible.
- **Detección de burnout**: un hilo monitor supervisa el estado de las personas y detecta cuando alguna supera su tiempo límite.
- **Serialización del log**: la salida por pantalla se protege con un mutex para evitar mensajes mezclados.

## Sincronización de hilos

La coordinación se apoya en primitivas de `pthread`:

- `pthread_mutex_t`
	- Protege los estados compartidos de los recursos.
	- Serializa la salida estándar.
	- Controla la bandera global de parada de la simulación.
- `pthread_cond_t`
	- Permite que los hilos esperen de forma pasiva cuando los recursos no están disponibles.
	- Despierta a los hilos cuando cambia el estado del sistema para que vuelvan a competir por los recursos.

## Tester

### Casos base

```bash
./codexion 5 800 200 200 200 3 10 fifo
./codexion 5 800 200 200 200 3 10 edf
```

### Codificador único

```bash
./codexion 1 800 200 200 200 3 10 fifo
```

### Dos codificadores

```bash
./codexion 2 800 200 200 200 3 10 fifo
```

### Burnout inevitable

```bash
./codexion 5 300 400 200 200 3 100 fifo
```

### Cero cooldown

```bash
./codexion 5 800 200 200 200 3 0 fifo
```

### Alta cooldown

```bash
./codexion 4 1500 200 200 200 3 500 fifo
```

### Alta contention

```bash
./codexion 10 1500 200 200 200 3 100 fifo
```

### EDF vs FIFO

```bash
./codexion 3 110 50 0 0 2 0 edf
./codexion 3 110 50 0 0 2 0 fifo
```
