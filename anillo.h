#ifndef ANILLO_H_
#define ANILLO_H_

#include <iostream>
#include <cassert>
using namespace std;

//se puede asumir que el tipo T tiene constructor por copia
//y operator==
//no se puede asumir que el tipo T tenga operator=
template<typename T>
class Anillo {
public:

    /*
     * constructor por defecto, crea un anillo
     */
    Anillo();

    /*
     * constructor por copia, una vez copiado, ambos anillos deben ser
     * independientes, es decir, cuando se borre uno no debe morir el otro.
     */
    Anillo(const Anillo<T>&);

    /*
     * Destructor. Acordarse de liberar toda la memoria!
     */
    ~Anillo();

    /*
     * Devuelve true si los anillos son iguales
     */
    bool operator==(const Anillo<T>&) const;

    /*
     * Dice si el anillo es vac�o.
     */
    bool esVacio() const;

    /*
     * Dice el tamanio del anillo
     */
    int tamanio() const;

    /*
     * Devuelve el pr�ximo elemento seg�n el orden del anillo.
     * El anillo debe rotar. Con lo cual sucesivas llamadas a esta funci�n
     * retornan distintos valores.
     *
     * El valor retornado pasa a ser el anterior.
     *
     * PRE: no es vac�o el anillo.
     */
    const T& siguiente();

    /*
     * Agrega el elemento al anillo. Recordar que el �ltimo agregado es el
     * pr�ximo en el orden
     */
    void agregar(const T&);

    /*
     * Elimina el elemento indicado del anillo.
     */
    void eliminar(const T&);

    /*
     * Indica si hubo anterior elemento elegido
     * y si �ste pertenece todav�a al anillo.
     */
    bool huboAnterior() const;

    /*
     * Indica cu�l es el elemento elegido la �ltima vez.
     */
    const T& anterior() const;

    /*
     * Vuelve hacia atr�s un elemento.
     * El anterior, en caso de existir, no debe alterarse.
     */
    void retroceder();

    /*
     * debe mostrar el anillo en el stream (y retornar el mismo).
     * Anillo vacio: []
     * Anillo con 2 elemento (e1 es el pr�ximo a aparecer en siguiente): [e1, e2]
     * Anillo con 2 elemento (e2 es el pr�ximo a aparecer en siguiente y e1 es el anterior): [e2, e1*]
     *
     */
    ostream& mostrarAnillo(ostream&) const;

private:
    //No se puede modificar esta funcion.
    Anillo<T>& operator=(const Anillo<T>& otro) {
        assert(false);
        return *this;
    }

    void setearUltimo(T &valor, bool sacoBasura);

    //Aca va la implementacion del nodo.
    struct Nodo {
        T *persona;
        Nodo *siguiente;
        Nodo *anterior;

//        Nodo(){
//            std::cout << "creado: " << this << std::endl;
//        }
        ~Nodo() {
//            std::cout << "borrado: " << this << std::endl;
            delete persona;
        }
    };

    Nodo *_actual;
    Nodo *_ultimo;
    Nodo *_ultimoEnLaFila;
    int _tam;


};

template<class T>
ostream& operator<<(ostream& out, const Anillo<T>& a) {
    return a.mostrarAnillo(out);
}


template<typename T>
Anillo<T>::Anillo() {
    _actual = NULL;
    _ultimo = NULL;
    _ultimoEnLaFila = NULL;
    _tam = 0;
}

template<typename T>
void Anillo<T>::setearUltimo (T & valor, bool sacoBasura) {
    int tam = _tam;
    Nodo *actual = _actual;
    while (actual != NULL && tam > 0) {
        if (*actual->persona == valor) {
            if (sacoBasura) _ultimo = actual;
        }
        actual = actual->siguiente;
        tam--;
    }
}

template<typename T>
Anillo<T>::Anillo(const Anillo<T> &otro) {
    _actual = NULL;
    _ultimo = NULL;
    _ultimoEnLaFila = NULL;
    _tam = 0;
    int tam = otro._tam;
    Nodo *actual = otro._ultimoEnLaFila;
    while (tam > 0) {
        agregar(*actual->persona);
        actual = actual->anterior;
        tam--;
    };
    if (otro._ultimo != NULL) setearUltimo(*otro._ultimo->persona, otro.huboAnterior());
};

template<typename T>
Anillo<T>::~Anillo() {
    Nodo *actual = _actual;
    int tam = _tam;
    while (actual != NULL && tam > 0) {
        Nodo *siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
        tam--;
    }
}

template<typename T>
bool Anillo<T>::esVacio() const {
    return _tam == 0;
}

template<typename T>
int Anillo<T>::tamanio() const {
    return _tam;
}


template<typename T>
const T &Anillo<T>::siguiente() {
    _ultimo = _actual;
    _ultimoEnLaFila = _actual;
    _actual = _actual->siguiente;

    return *_ultimo->persona;
}

template<typename T>
void Anillo<T>::agregar(const T &otro) {
    Nodo *nuevo = new Nodo();
    nuevo->persona = new T(otro);
    nuevo->siguiente = _actual;

    if (_tam == 0) {
        _ultimoEnLaFila = nuevo;
    } else {
        _actual->anterior = nuevo;
    }
    _ultimoEnLaFila->siguiente = nuevo;

    nuevo->anterior = _ultimoEnLaFila;

    _actual = nuevo;

    _tam++;
}


template<typename T>
void Anillo<T>::eliminar(const T &p) {
    if (_tam == 1) {
        delete _actual;
        _actual = NULL;
        _ultimoEnLaFila = NULL;
        _tam = 0;
    } else {
        int tam = _tam;
        Nodo *anterior = _actual;
        if (*_actual->persona == p) {
            _actual = _actual->siguiente;
            _ultimoEnLaFila->siguiente = _actual;
            if (huboAnterior() && *_ultimo->persona == p) _ultimo = NULL;
            if (*_ultimoEnLaFila->persona == p) _ultimoEnLaFila->anterior;
            delete anterior;
            _tam--;
        } else {
            while (tam > 0) {
                Nodo* actual = anterior->siguiente;
                if (*actual->persona == p) {
                    anterior->siguiente = actual->siguiente;
                    actual->siguiente->anterior = anterior;
                    if (huboAnterior() && *_ultimo->persona == p) _ultimo = NULL;
                    if (*_ultimoEnLaFila->persona == p) _ultimoEnLaFila = NULL;
                    delete actual;
                    _tam--;
                    break;
                }
                anterior = actual;
                tam--;
            }
        }
    }

}

template<typename T>
bool Anillo<T>::huboAnterior() const {
    return _ultimo != NULL;
}

template<typename T>
const T &Anillo<T>::anterior() const {
    return *_ultimo->persona;
}

template<typename T>
void Anillo<T>::retroceder() {
    _actual = _actual->anterior;
//    _ultimo = _actual->anterior;
    _ultimoEnLaFila = _ultimo;
}

template<typename T>
ostream &Anillo<T>::mostrarAnillo(ostream &os) const {
    Nodo *actual = _actual;
    int tam = _tam;
    os << "[";
    if (tam > 0) {
        os << *actual->persona;
        if (huboAnterior() && *_ultimo->persona == *actual->persona) {
            os << "*";
        }
        actual->siguiente;
    }
    while (tam > 1) {
        actual = actual->siguiente;
        os << ", " << *actual->persona;
        if (huboAnterior() && *_ultimo->persona == *actual->persona) {
            os << "*";
        }
        tam--;
    }
    os << "]";
}

template<typename T>
bool Anillo<T>::operator==(const Anillo<T> &otro) const {
    Nodo *actualA = _actual;
    Nodo *actualB = otro._actual;
    if (otro._tam != _tam) return false;
    int tam = _tam;
    while (tam > 0) {
        if (*actualA->persona != *actualB->persona) return false;
        actualA = actualA->siguiente;
        actualB = actualB->siguiente;
        tam--;
    }
    if (*_ultimo->persona != *otro._ultimo->persona) return false;
    return true;
}

#endif //ANILLO_H_
