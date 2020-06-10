#ifndef TRAVELLERSAPP_CSV_HPP
#define TRAVELLERSAPP_CSV_HPP

#include "../SimpleString.hpp"
#include "../collection/ArrayList.hpp"
#include "../collection/Tuple.hpp"
#include "../ttype.hpp"
#include "Scanner.hpp"
#include <fstream>

/**
 * An ill-conceived attempt to create a csv file.
 * It's closer to a SSV (Space Separated Values) to a CSV
 * since no commas are actually used in the files.
 * The element in the first column is considered a key and must be unique.
 * It can store an arbitrary number of arbitrary types... theoretically
 * It doesn't actually work with anything that isn't a char* or a class that
 * supports implicit conversion from char* (Such as {@link SimpleString}).
 * It does an adequate job for the types it does work for.
 * And it does make sure at compile-time that the exact amount of entries
 * required are  used.
 * @bug It ignores the actual header names inside the file. It just cares about
 * their count.
 */
template <typename... Types> class CSV {
private:
  /// The number of columns
  static constexpr int columns = sizeof...(Types);
  static_assert(columns > 0, "CSV file needs at least one column");
  /// The type of the header - A {@link Tuple} with as many const char*
  /// provided as type arguments as there are columns.
  using CSVHeader = typename Tuple<const char *>::ofSize<columns>::type;
  CSVHeader _header;
  using Entry = Tuple<Types...>;
  ArrayList<Entry> entries;

public:
  /// It is necessary to provide the names of the header manually.
  /// This may change in the future.
  CSV() = delete;

  /// Construct a new csv file with the provided column header.
  CSV(CSVHeader const &header) : _header(header){};

  /// Allow the passing of a column-amount of const char*
  /// instead of a tuple for the construction of the header.
  template <typename... Ts> CSV(Ts... ts) : _header(CSVHeader(ts...)) {}

  /** Load the entries from the CSV file located at the given path
   * The number of columns in the file must match the number of columns of
   * this class.
   * @bug Ignores the header.
   * @bug This doesn't really work as advertised. It doesn't check if each
   * row contains the exact amount of columns as this class.
   * Rather it checks that the total number of separate entries
   * (including the header) is evenly divisible by the amount of columns.
   * EXAMPLE: Assuming that this class expects a file like this
   *******************************
   * header1 * header2 * header3 *
   * entry1  * entry2  * entry3  *
   *******************************
   * A file formatted like so:
   *****************************************
   * header1 * header2 * header3 * header4 *
   * entry11 * entry12 * entry13 * entry14 *
   * entry21 * entry22 * entry23 * entry24 *
   *****************************************
   * Will be considered valid, and will be loaded like so
   *******************************
   * header1 * header2 * header3 *
   * header4 * entry11 * entry12 *
   * entry13 * entry14 * entry21 *
   * entry22 * entry23 * entry24 *
   *******************************
   */
  void load(const char *filepath) {
      std::ifstream file(filepath);
    if (file.is_open()) {
      Travel::CommandList cl;
      Travel::Scanner scanner(ScannerContext::ALL, &file, &cl);
      auto tokens =
          scanner.scan()
              ->filter([](Token const &t) { return t.t != TokenType::EOF_T; })
              ->template map<typename Entry::template typeOf<0>::type>(
                  [](Token const &t) {
                    char *s = t.lexeme;
                    char *d = new char[strlen(s) + 1];
                    strcpy(d, s);
                    return d;
                  });
      if (tokens->length() < columns) {
        std::cerr << filepath << " is empty" << std::endl;
        file.close();
        return;
      }
      for (int i = 0; i < columns; i++) {
        delete[] tokens->get(i);
      }
      std::unique_ptr<ArrayList<Tuple<Types...>>> readEntries =
          tokens->template slice<columns>();
      for (int i = 0; i < readEntries->length(); i++) {
        auto k = getKeys();
        if (k->find([&readEntries, i](const char *const(&entry)) {
              return strcmp(readEntries->get(i).head(), entry) == 0;
            }) >= 0) {
          std::cerr << "Key " << readEntries->get(i).head()
                    << " already exists. Skipping" << std::endl;
          readEntries->get(i).free();
          continue;
        }
        entries.append(readEntries->get(i));
      }
    } else {
      std::cerr << "Cannot open " << filepath << " for read." << std::endl;
    }
    file.close();
  }

  /**
   * Save the contents of the file into the file pointed to
   * by the provided path. The file is formatted properly so that
   * it can later be read by this class' {@link load} function
   */
  void save(const char *filepath) {
      std::ofstream file(filepath);
    if (file.is_open()) {
      file << header() << std::endl;
      for (int i = 0; i < size(); i++) {
        file << entries.get(i) << std::endl;
      }
    } else {
      std::cerr << "Cannot open " << filepath << " for write." << std::endl;
    }
    file.close();
  }

  /// Return the header of this file
  CSVHeader header() { return _header; }

  /// Add an entry. This will make sure at compile-time that the amount of
  /// elements provided matches the amount of columns in the file.
  void addEntry(Types &... data) { entries.append({data...}); }

  /// Returns a reference to the row at the given index.
  Tuple<Types...> &getRow(unsigned ind) { return entries.get(ind); }

  /// Returns the amount of entries in the file.
  unsigned size() { return entries.length(); }

  /// Returns the first column of the file.
  auto getKeys()
      -> std::unique_ptr<ArrayList<typename Entry::template typeOf<0>::type>> {
    return getColumn<0>();
  }

  /// Returns the specified column.
  /// @tparam ind the column to return.
  template <int ind>
  auto getColumn()
      -> std::unique_ptr<ArrayList<typename Entry::template typeOf<ind>::type>> {
    return entries.template map<typename Entry::template typeOf<ind>::type>(
        [](Entry const &t) { return *t.template get<ind>(); });
  }

  ~CSV() {
    for (int i = 0; i < entries.length(); i++) {
      entries.get(i).free();
    }
  }

  /// Return the column with the given header
  /// I am not entirely sure this works.
  /// It did when I was testing while writing it but...
  auto getByColumnHeader(const char *s) {
    int ind = header().template indexOf<const char *>(s);
    if (entries.length() == 0) {
      throw NoValueException("CSV contains no entries");
    }
    auto t = *entries.get(0).template at<decltype(s)>(ind);
    return entries.template map<decltype(t)>(
        [ind](const Entry &e) { return *e.template at<decltype(s)>(ind); });
  }
};
#endif // TRAVELLERSAPP_CSV_HPP
