#include <iostream>
#include <cstdlib>
#include <array>
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
    Toy () {}
    virtual void isTaken() = 0;
    void serialize(){}
};

class LittlePony : public Toy {
  public:
    LittlePony() {}
    LittlePony(std::string n){
      name = n;
    }

    void isTaken(){
      std::cout << "yo man" << std::endl;
    }
    void serialize(){}
};

class Teddy : public Toy {
  public:
    Teddy () {}
    Teddy(std::string n){
      name = n;
    }

    void isTaken(){
      std::cout << "gra hu" << std::endl;
    }
    void serialize(){}
};

class Wrap : public Object {
  protected:
    Object * wrapped_object;
    bool isOpen;
    bool isEmpty;
  public:
    virtual int wrapMeThat(Object * object) = 0;
    
    Object * openMe(){
      if (!isEmpty){
        isOpen = true;
        return wrapped_object;
      }
      else {
        std::cerr << "No object inside. Nullprt is returned " << std::endl;;
        isOpen = true;
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
    Box () {
      Object * wrapped_object = nullptr;
      isEmpty = true;
      isOpen = false;
    }

    int wrapMeThat(Object * object){
      if (!isEmpty){
        std::cerr << "Too much objects for one wrap" << std::endl;
        return 0;
      }
      if (isOpen){ 
        wrapped_object = object;
        isOpen = false;
        isEmpty = false; 
        return 1;
      }
      else {
        std::cerr << "Box is closed. Must be open " << std::endl;;
        return 0;
      }   
    }
};

class GiftPaper : public Wrap {
  public:
    GiftPaper() {
      Object * wrapped_object = nullptr;
      isEmpty = true;
    }
    int wrapMeThat(Object * object){
      if (!isEmpty){
        std::cerr << "Too much objects for one wrap " << std::endl;;
        return 0;
      }
      wrapped_object = object;
      isEmpty = false;
      return 1; 
    }
};

class ITable {
  public: 
    virtual int put(Object * o) = 0;
    virtual Object * give(int obj_num) = 0;
};

class Table : public ITable {
  protected:
    const static int max_objects = 10;
    Object * objects[max_objects];
    int last_free;

  public:
    Table () {
      for (int i = 0; i < max_objects; i++) {
            objects[i] = nullptr;
        }
      last_free = 0;
    }
    
    int put(Object * o){
      if (last_free < max_objects){
        objects[last_free] = o;
        last_free++;
        return 1;
      }
      else {
        std::cerr << "Too many objects on the table! " << std::endl;;
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

class IConveyorBelt {
  public:
    virtual int put(Object * o) = 0;
    virtual Object * give() = 0;
    virtual Object * out() = 0;
    virtual Object * in() = 0;
};

class ConveyorBelt : public Object, public IConveyorBelt {
  protected:
    Object * object_on;
  
  public:
    ConveyorBelt () {}

    int put(Object * o){
      if (object_on != nullptr){
        std::cerr << "Only 1 object on ConveyorBelt is allowed " << std::endl;;
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
      Object * temp = object_on;
      object_on = nullptr;
      return temp;
    }

    Wrap * in(){
      Wrap * randomWrap;
      int r = rand() % 2;
      switch (r) {
        case 0: {
          Box *box = new Box();
          box->openMe();
          randomWrap = dynamic_cast<Wrap*>(box);
          break;
        }
        case 1: {
          GiftPaper *giftPaper = new GiftPaper();
          randomWrap = dynamic_cast<Wrap*>(giftPaper);
          break;
        }
      }
      return randomWrap;
    }

    void serialize(){}
};

class IElf {
  public:
    virtual ~IElf() {}

    virtual std::string get_name() = 0;
    virtual void set_name(std::string s) = 0;

    virtual int put(Object * what, Object * where_to) = 0;
    virtual Object * take(Table * t, int obj_num) = 0;
    virtual Object * take(ConveyorBelt * cb) = 0;
    virtual int wrap(Object * gift, ConveyorBelt * cb) = 0;
    virtual std::array<std::string,11> look(Table * t) = 0;
    virtual Object * send(ConveyorBelt * cb) = 0;
};

class PapaXmasElf : public IElf {
  protected:
    std::string name;

  public:
    PapaXmasElf(std::string n){
      name = n;
    }

    ~PapaXmasElf() {}

    std::string get_name(){
      return name;
    };

    void set_name(std::string s){
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
      std::cerr << "No table or conveyor were provided! " << std::endl;;
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
      std::cout << "whistles while working " << std::endl;;
      if (wr->wrapMeThat(gift)){
        cb->put(wr);
        std::cout << "tuuuuttuuuttuut " << std::endl;;
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
        std::cerr << "No gift to send! " << std::endl;;
        return nullptr;
      }
    }
};

Object ** myUnitTests(){
  LittlePony * lp = new LittlePony("happy pony");
  Teddy * td = new Teddy("cuddles");
  Object ** arrayOut = new Object*[2];
  arrayOut[0] = lp;
  arrayOut[1] = td;
  return arrayOut;
}

Object *MyUnitTests(Object **objects) {
    if (objects == nullptr || objects[0] == nullptr || objects[1] == nullptr || objects[2] == nullptr) {
        std::cerr << "One of objects is null!" << std::endl;
        return nullptr;
    }

    Teddy *teddy = dynamic_cast<Teddy*>(objects[0]);
    Box *box = dynamic_cast<Box*>(objects[1]);
    GiftPaper *giftPaper = dynamic_cast<GiftPaper*>(objects[2]);

    if (teddy == nullptr || box == nullptr || giftPaper == nullptr) {
        std::cerr << "Invalid object types in array!" << std::endl;;
        return nullptr;
    }

    box->openMe();

    if (!box->wrapMeThat(teddy)) {
        std::cerr << "Failed to put teddy in the box!" << std::endl;
        return nullptr;
    }

    if (!giftPaper->wrapMeThat(box)) {
        std::cerr << "Failed to wrap box in gift!" << std::endl;
        return nullptr;
    }

    return giftPaper;
}

ITable* createTable() {
  Table* table = new Table();

  table->put(new Teddy("Teddy 1"));
  table->put(new LittlePony("Little Pony 1"));

  return table;
}

IConveyorBelt* createConveyorBelt() {
  ConveyorBelt* conveyorBelt = new ConveyorBelt();

  return static_cast<IConveyorBelt*>(conveyorBelt);
}


int main() {
  std::cout << "------ 1 ------" << std::endl;
  // first test
  Object ** arrayOut = new Object*[2];
  arrayOut = myUnitTests();
  std::cout << arrayOut[0]->getName() << std::endl;
  std::cout << arrayOut[1]->getName() << std::endl;

  std::cout << "------ 2 ------" << std::endl;

  // second test
  Teddy teddy("Teddy Bear");
  Box box;
  GiftPaper gift_paper;
  Object* objects[] = { &teddy, &box, &gift_paper, nullptr };
  Object* present = MyUnitTests(objects);
  Wrap * pr = dynamic_cast<Wrap*>(present);
  Wrap * p = dynamic_cast<Wrap*>(pr->openMe());
  if (present != nullptr) {
      std::cout << "Final present is: " << p->openMe()->getName() << std::endl;
  } else {
      std::cout << "No present was created." << std::endl;
  }

  std::cout << "------ 3 ------" << std::endl;

  //third test 
  ITable *table = createTable();
  IConveyorBelt *conveyorBelt = createConveyorBelt();
  ConveyorBelt* cb = dynamic_cast<ConveyorBelt*>(conveyorBelt);

  Toy *toy1 = new LittlePony();
  Toy *toy2 = new Teddy();

  table->put(toy1);
  table->put(toy2);

  PapaXmasElf *elf = new PapaXmasElf("Elf");
  
  elf->wrap(table->give(0), cb);
  Wrap * w = dynamic_cast<Wrap*>(elf->send(cb));
  std::cout << w->openMe()->getName() << std::endl;
  
  elf->wrap(table->give(1), cb);
  w = dynamic_cast<Wrap*>(elf->send(cb));
  std::cout << w->openMe()->getName() << std::endl;
  
  return 0;
}