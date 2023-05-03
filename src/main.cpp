#include <iostream>
#include <cstdlib>
#include<array>
#include <string>

class Object {
  protected:
    std::string name;
  public:
    virtual void serialize() = 0;
    
    friend std::ostream& operator << (std::ostream& os, const Object &obj){
      os << obj.name;
      return os;
    }

    std::string getName(){
      return name;
    }
};



class Toy : public Object {
  public:
    virtual void isTaken();
};

class LittlePony : public Toy {
  public:
    void isTaken(){
      std::cout << "yo man" << std::endl;
    }
};

class Teddy : public Toy {
  public:
    void isTaken(){
      std::cout << "gra hu" << std::endl;
    }
};

class Wrap : public Object {
  protected:
    Object * wrapped_object;
    bool isOpen;
  public:
    int wrapMeThat(Object * object){
      if (wrapped_object != nullptr){
        std::cerr << "Too much objects for one wrap \n";
        return 0;
      }
      wrapped_object = object;
      return 1; 
    }
    
    Object * openMe(){
      if (wrapped_object != nullptr){
        isOpen = true;
        return wrapped_object;
      }
      else {
        std::cerr << "No object inside. Nullprt is returned \n";
        return nullptr;
      }
    }

    void closeMe(){
      isOpen = false;
    }

    void serialize(){};
};

class Box : public Wrap {
  public:
    int wrapMeThat(Object * object){
      if (wrapped_object != nullptr){
        std::cerr << "Too much objects for one wrap \n";
        return 0;
      }
      if (isOpen){ 
        wrapped_object = object;
        isOpen = false; 
        return 1;
      }
      else {
        std::cerr << "Box is closed. Must be open \n";
        return 0;
      }   
    }
};

class GiftPaper : public Wrap {};

class ITable {
  public: 
    virtual int put(Object * o) = 0;
    virtual Object * give(int obj_num) = 0;
};

class IConveyorBelt {
  public:
    virtual int put(Object * o) = 0;
    virtual Object * give() = 0;
};

class Table : public ITable {
  const static int max_objects = 10;
  Object * objects[max_objects];
  int last_free = 0;

  public:
    Table () {}
    int put(Object * o){
      if (last_free < max_objects){
        objects[last_free] = o;
        last_free++;
        return 1;
      }
      else {
        std::cerr << "Too many objects on the table! \n";
        return 0;
      }
    }

    Object * give(int obj_num){
      return objects[obj_num];
    }

    int getLastFree(){
      return last_free;
    }
};

class ConveyorBelt : public Object, public IConveyorBelt {
  protected:
    Object * object_on;
  
  public:
    int put(Object * o){
      if (object_on != nullptr){
        std::cerr << "Only 1 object on ConveyorBelt is allowed \n";
        return 0;
      }
      else {
        object_on = o;
        return 1;
      }
    }

    Object * give(){
      return object_on;
    }

    Object * out(){
      object_on = NULL;
      return object_on;
    }

    Wrap * in(){
      Wrap * randomWrap;
      int r = rand() % 2;
      switch (r) {
        case 0:
          randomWrap = dynamic_cast<Box*>(randomWrap);
          break;
        case 1:
          randomWrap = dynamic_cast<GiftPaper*>(randomWrap);
          break;
      }
      return randomWrap;
    }


};

class IElf {
  public:
    virtual std::string get_name();
    virtual std::string set_name(std::string s);

    virtual int put(Object * what, Object * where_to) = 0;
    virtual Object * take(Table * t, int obj_num) = 0;
    virtual Object * take(ConveyorBelt * cb) = 0;
    virtual int wrap(Object * gift, Object * wrapper) = 0;
    virtual std::array<std::string,11> look(Table * t) = 0;
};

class PapaXmasElf : public IElf {
  std::string name;

  public:
    PapaXmasElf(std::string n){
      name = n;
    }

    std::string get_name(){
      return name;
    };

    std::string set_name(std::string s){
      name = s;
    };

    int put(Object * what, Object * where_to){
      Table * t = dynamic_cast<Table*>(where_to);
      ConveyorBelt * cb = dynamic_cast<ConveyorBelt*>(where_to);

      if (t != nullptr){
        if (t->put(what))
          return 1;
        return 0;
      }
      if (cb != nullptr){
        if (cb->put(what))
          return 1;
        return 0;
      }
      std::cerr << "No table or conveyor were provided! \n";
      return 0;
    };
    
    Object * take(Table * t, int obj_num){
      return t->give(obj_num);
    }
    
    Object * take(ConveyorBelt * cb){
      return cb->give();
    }
    
    int wrap(Object * gift, ConveyorBelt * cb){
      Wrap * wr = cb->in();
      std::cout << "whistles while working \n";
      if (wr->wrapMeThat(gift)){
        std::cout << "tuuuuttuuuttuut \n";
        return 1;
      }
      return 0;
    }

    std::array<std::string,11> look(Table * t){
      std::array<std::string,11> outArray;
      for (int i = 0; i < t->getLastFree(); i++){
        outArray[i] = t->give(i)->getName();
      }
      outArray[t->getLastFree()] = nullptr;
      return outArray;
    }

    Object * send(ConveyorBelt * cb){
      Object * obj = cb->out();
      if (obj != nullptr)
        return obj;
      else {
        std::cerr << "No gift to send! \n";
        return nullptr;
      }
    }
};

int main() {
  std::cout << "Hello World! \n";
  return 0;
}