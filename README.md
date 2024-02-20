# Proyecto-Supermercado-Hipermod
Proyecto para tecnicas de programación 2.
Enunciado:
El nuevo supermercado HiperMod, nos ha solicitado la elaboración del software para la gestión y manejo de una
nueva caja automatizada, donde los clientes podrán seleccionar el producto y automáticamente se le agregara a
un carrito de compra para luego despachar cuando complete el proceso de compra.
Especificaciones:
1. Tendrá un módulo de productos específicos a cargar el sistema automatizado, cada producto debe tener su
debida identificación y descripción, stock y precio de venta.
2. El cliente podrá realizar la selección de los productos y cantidad por cada producto, el carrito no puede
superar el máximo de 30 artículos/productos, por cada tipo de producto solo podrá llevarse una máximo de 5
artículos.
3. Los artículos se introducen en el carrito de compras obedeciendo a una estructura de tipo pila de 10
items/elementos, donde cada elemento de la pila es un tipo de producto.
4. El proceso para seleccionar productos, cargar el carrito y realizar la compra solo puede durar un máximo de
10 minutos, si el cliente supera el máximo de tiempo para realizar la compra el sistema lo enviará al final de
la cola y le dará la oportunidad al siguiente cliente en espera.
5. Los clientes se generarán de forma automática y se irán formando en una cola para esperar su turno (estimar
tiempo de llegada de clientes de 4 a 6 minutos).
6. Imprimir ticket de compra: muestre por pantalla Datos del cliente(cedula, nombre y apellido, telefono), el
contenido del carrito (referencia, nombre, precio unitario, cantidad y precio) y la totalización del carrito.
7. Se deben generar los siguientes reportes:
 Cantidad de Clientes que se presentaron a comprar.
 Cantidad de Clientes con compras realizadas.
 Clientes que superaron el máximo de tiempo para comprar
 Producto más vendido.
 Totales de ventas.
 Guardar/mostrar ultimas 10 facturas/tickets
8. El proceso de la Caja Automatizada sera simulado, tomar en cuenta las siguientes premisas:
 Se debe contar con un menu principal para iniciar el proceso de las compras de los clientes.
 El ticket de compra se debe mostrar en pantalla hasta que otro cliente termine su compra.
 Poder terminar la simulacion en cualquier momento y mostrar los reportes.
9. Se valorará:
 Una buena modularizacion (use modulos/procedimientos/funciones)
 Uso de tipos/estructuras de datos adecuados.
 La ausencia de efectos laterales.
 La legibilidad del código.
 La presencia y claridad de mensajes de salida, resultado de cada operación.
