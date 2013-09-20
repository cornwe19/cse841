class City {
   public:
      City( FILE* file ) {
         _name = new char[128];
         fscanf( file, "%s (%d,%d)", _name, &_x, &_y );
      }
      
      ~City() {
         delete [] _name;
         _name = NULL;
      }

      int getX() {
         return _x;
      }

      int getY() {
         return _y;
      }

      char* getName() {
         return _name;
      }

      bool isHub() {
         return _name[0] == '*';
      }
   
   private:
      char* _name;
      int   _x, _y;
};
