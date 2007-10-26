#include <sqlite3.h>

namespace stactiverecord {
  using namespace std;

  enum coltype { INTEGER, STRING, RECORD };

  class Sar_Dbi {
  public:
    static Sar_Dbi* makeStorage(string config);
    static Sar_Dbi* dbi;
    vector<string> initialized_tables;
    Sar_Dbi() : initialized_tables() {};
    virtual void delete_record(int id, string classname) {};
    virtual void initialize_tables(string classname) {};
    virtual int next_id(string classname) {};
    virtual void get(int id, string classname, SarMap<string>& values) {};
    virtual void get(int id, string classname, SarMap<int>& values) {};
    virtual void set(int id, string classname, SarMap<string> values, bool insert) {};
    virtual void set(int id, string classname, SarMap<int> values, bool insert) {};
    bool table_is_initialized(string tablename);
  };

  class SQLiteStorage : public Sar_Dbi {
  private:
    void close();
    sqlite3 *db;
    bool is_closed;
    void test_result(int result, const string& context);
    void execute(string query);
  public:
    SQLiteStorage(string location);
    ~SQLiteStorage() { close(); };
    int next_id(string classname);
    void delete_record(int id, string classname);
    void initialize_tables(string classname);
    void get(int id, string classname, SarMap<string>& values);
    void get(int id, string classname, SarMap<int>& values);
    void set(int id, string classname, SarMap<string> values, bool insert);
    void set(int id, string classname, SarMap<int> values, bool insert);
  };

  /*
  class BDBStorage : public Sar_Dbi {
  public:
    void insert_or_update(int id, string classname, map<string,string> keyvalues);
    void delete_record(int id, string classname);
    void initialize_tables(string classname);
  };
  */

};
