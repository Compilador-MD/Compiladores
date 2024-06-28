#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <sstream>

using namespace std;

int pos = 0;

struct Token {
    string token_type;
    string token_value;
    int line;
    int column;
    string aux_value;
    void print_token() {
        cout << "Token: " << token_type << " Value: " << token_value
            << " Linea: " << line << " pos: " << column << endl;
    }

    void operator = (const Token& t) {
        token_type = t.token_type;
        token_value = t.token_value;
        line = t.line;
        column = t.column;
        aux_value = t.column;
    }
};

//LL(1)
struct Production
{
    vector<string> production;
    Production(initializer_list<string> prod) : production(prod) {}
};
struct LL1ParserTable
{
    map<pair<string, string>, Production> table;
    void printProduction(const Production& rule) const
    {
        for (const auto& symbol : rule.production)
            cout << symbol << " ";
    }

    void addRule(const string& NT, const string& T, const Production& rule) {
        table.emplace(make_pair(NT, T), rule);
    }
    void printTable() const {
        for (const auto& entry : table) {
            cout << "NT: " << entry.first.first
                << ", T: " << entry.first.second
                << " => ";
            printProduction(entry.second);
            cout << endl;
        }
    }
};
void AddRules(LL1ParserTable& parserTable)
{
    //S'
    parserTable.addRule("S'", "$", { "documento", "$" });
    parserTable.addRule("S'", "<", { "documento", "$" });

    //documento
    parserTable.addRule("documento", "$", { "bloques" });
    parserTable.addRule("documento", "<", { "bloques" });

    //bloques
    parserTable.addRule("bloques", "$", { "epsilon" });
    parserTable.addRule("bloques", "<", { "<", "bloque", ">", "bloquesP" });

    //bloqueP
    parserTable.addRule("bloquesP", "$", { "epsilon" });
    parserTable.addRule("bloquesP", "<", { "<", "bloque", ">", "bloquesP" });

    //bloque
    parserTable.addRule("bloque", "<", { "texto" }); //AGREGADO
    parserTable.addRule("bloque", ">", { "texto" });
    parserTable.addRule("bloque", "#-", { "header" });
    parserTable.addRule("bloque", "##-", { "header" });
    parserTable.addRule("bloque", "###-", { "header" });
    parserTable.addRule("bloque", "LETRAS", { "texto" });
    parserTable.addRule("bloque", "*-", { "texto" });
    parserTable.addRule("bloque", "/-", { "texto" });
    parserTable.addRule("bloque", "_-", { "texto" });
    parserTable.addRule("bloque", "~-", { "texto" });
    parserTable.addRule("bloque", "!-", { "lista" });
    parserTable.addRule("bloque", "/*", { "comentario" });
    parserTable.addRule("bloque", "/url:", { "url" });
    parserTable.addRule("bloque", "/img:", { "imagenes" });
    parserTable.addRule("bloque", "{-", { "tabla" });
    parserTable.addRule("bloque", "|-", { "tabla" });
    parserTable.addRule("bloque", "%", { "tabla" });

    //header
    parserTable.addRule("header", "#-", { "H1" });
    parserTable.addRule("header", "##-", { "H2" });
    parserTable.addRule("header", "###-", { "H3" });

    //H1 H2 H3
    parserTable.addRule("H1", "#-", { "#-", "texto", "-#" });
    parserTable.addRule("H2", "##-", { "##-", "texto", "-##" });
    parserTable.addRule("H3", "###-", { "###-", "texto", "-###" });

    //texto
    parserTable.addRule("texto", "LETRAS", { "LETRAS" });
    parserTable.addRule("texto", "*-", { "estilo", "texto" });
    parserTable.addRule("texto", "-*", { "epsilon" });
    parserTable.addRule("texto", "/-", { "estilo", "texto" });
    parserTable.addRule("texto", "-/", { "epsilon" });
    parserTable.addRule("texto", "_-", { "estilo", "texto" });
    parserTable.addRule("texto", "-_", { "epsilon" });
    parserTable.addRule("texto", "~-", { "estilo", "texto" });
    parserTable.addRule("texto", "-~", { "epsilon" });
    parserTable.addRule("texto", "-!", { "epsilon" });
    parserTable.addRule("texto", "*/", { "epsilon" });
    parserTable.addRule("texto", ":/", { "epsilon" });
    parserTable.addRule("texto", ">", { "epsilon" });
    parserTable.addRule("texto", "-#", { "epsilon" });
    parserTable.addRule("texto", "-##", { "epsilon" });
    parserTable.addRule("texto", "-###", { "epsilon" });
    parserTable.addRule("texto", ".png/", { "epsilon" });
    parserTable.addRule("texto", ".jpg/", { "epsilon" });
    parserTable.addRule("texto", "-}", { "epsilon" });
    parserTable.addRule("texto", "-|", { "epsilon" });

    //estilo
    parserTable.addRule("estilo", "*-", { "bold" });
    parserTable.addRule("estilo", "/-", { "italic" });
    parserTable.addRule("estilo", "_-", { "underline" });
    parserTable.addRule("estilo", "~-", { "strikethrough" });

    //bold, italic, underline, strikethrough
    parserTable.addRule("bold", "*-", { "*-", "texto", "-*" });
    parserTable.addRule("italic", "/-", { "/-", "texto", "-/" });
    parserTable.addRule("underline", "_-", { "_-", "texto", "-_" });
    parserTable.addRule("strikethrough", "~-", { "~-", "texto", "-~" });

    //listas
    parserTable.addRule("lista", "!-", { "item", "listaP" });
    parserTable.addRule("listaP", ">", { "epsilon" });
    parserTable.addRule("listaP", "!-", { "item", "listaP" });

    //item
    parserTable.addRule("item", "!-", { "!-", "texto", "-!" });

    //comentario
    parserTable.addRule("comentario", "/*", { "/*", "texto", "*/" });

    //url
    parserTable.addRule("url", "/url:", { "/url:", "texto", ":/" });

    //img
    parserTable.addRule("imagenes", "/img:", { "/img:", "texto", "finImg" });

    //finImg
    parserTable.addRule("finImg", ".png/", { ".png/" });
    parserTable.addRule("finImg", ".jpg/", { ".jpg/" });

    //tablas
    parserTable.addRule("tabla", "{-", { "head", "fila", "tablaP" });
    parserTable.addRule("tabla", "%", { "head", "fila", "tablaP" });
    parserTable.addRule("tabla", "|-", { "head", "fila", "tablaP" });
    parserTable.addRule("tablaP", ">", { "epsilon" });
    parserTable.addRule("tablaP", "|-", { "fila", "tablaP" });
    parserTable.addRule("fila", "|-", { "celda", "filaP" });
    parserTable.addRule("filaP", ">", { "epsilon" });
    parserTable.addRule("filaP", "&", { "&", "celda", "filaP" });
    parserTable.addRule("filaP", "|-", { "epsilon" });
    parserTable.addRule("celda", "|-", { "|-", "texto", "-|" });

    //HEAD TABLA
    parserTable.addRule("head", "{-", { "hd", "headP" });
    parserTable.addRule("head", "|-", { "hd", "headP" });
    parserTable.addRule("head", "%", { "hd", "headP" });
    parserTable.addRule("headP", "%", { "%", "hd", "headP" });
    parserTable.addRule("headP", "|-", { "epsilon" });
    parserTable.addRule("hd", "{-", { "{-", "texto", "-}" });
    parserTable.addRule("hd", "|-", { "epsilon" });
    parserTable.addRule("hd", "%", { "epsilon" });
}

struct FF
{
    map<string, string> FF_table;
    void addFF(string NT, string FF) {
        FF_table.emplace(NT, FF);
    }
    void printFFTable(int cond) {
        for (auto& entry : FF_table) {
            string NT = entry.first;
            string _FF = entry.second;
            cout << "Token: " << NT;
            if (cond)
                cout << " | First: " << _FF;
            else
                cout << " | Follow: " << _FF;
            cout << endl;
        }
    }
};
void AddFirst(FF& first)
{
    //S'
    first.addFF("S'", "$");
    first.addFF("S'", "<");
    //documento
    first.addFF("documento", "<");
    //bloques
    first.addFF("bloques", "<");
    //bloquesP
    first.addFF("bloquesP", "<");
    //bloque
    first.addFF("bloque", "LETRAS");
    first.addFF("bloque", "/*");
    first.addFF("bloque", "<");  // AGREGADO
    first.addFF("bloque", "/img:");
    first.addFF("bloque", "/url:");
    first.addFF("bloque", "#-");
    first.addFF("bloque", "##-");
    first.addFF("bloque", "###-");
    first.addFF("bloque", "!-");
    first.addFF("bloque", "*-");
    first.addFF("bloque", "/-");
    first.addFF("bloque", "_-");
    first.addFF("bloque", "~-");
    first.addFF("bloque", "{-");
    first.addFF("bloque", "|-");
    first.addFF("bloque", "%");
    //header
    first.addFF("header", "#-");
    first.addFF("header", "##-");
    first.addFF("header", "###-");
    //H1
    first.addFF("H1", "#-");
    //H2
    first.addFF("H2", "##-");
    //H3
    first.addFF("H3", "###-");
    //texto
    first.addFF("texto", "LETRAS");
    first.addFF("texto", "*-");
    first.addFF("texto", "/-");
    first.addFF("texto", "_-");
    first.addFF("texto", "~-");
    //estilo
    first.addFF("estilo", "*-");
    first.addFF("estilo", "/-");
    first.addFF("estilo", "_-");
    first.addFF("estilo", "~-");
    //bold
    first.addFF("bold", "*-");
    //italic
    first.addFF("italic", "/-");
    //underline
    first.addFF("underline", "_-");
    //strikethrough
    first.addFF("strikethrough", "~-");
    //lista
    first.addFF("lista", "!-");
    //listaP
    first.addFF("listaP", "!-");
    //item
    first.addFF("item", "!-");
    //comentario
    first.addFF("comentario", "/*");
    //url
    first.addFF("url", "/url:");
    //imagenes
    first.addFF("imagenes", "/img:");
    //finImg
    first.addFF("finImg", ".png/");
    first.addFF("finImg", ".jpg/");
    //tabla
    first.addFF("tabla", "{-");
    first.addFF("tabla", "|-");
    first.addFF("tabla", "%");
    //tablaP
    first.addFF("tablaP", "|-");
    //fila
    first.addFF("fila", "|-");
    //filaP
    first.addFF("filaP", "&");
    //celda
    first.addFF("celda", "|-");
    //head
    first.addFF("head", "{-");
    first.addFF("head", "%");
    //headP
    first.addFF("headP", "%");
    //hd
    first.addFF("hd", "{-");
}
void AddFollow(FF& follow)
{
    //S'
    follow.addFF("S'", " ");
    //documento
    follow.addFF("documento'", "$");
    //bloques
    follow.addFF("bloques'", "$");
    //bloquesP
    follow.addFF("bloquesP'", "$");
    //bloque
    follow.addFF("bloque'", ">");
    //header
    follow.addFF("header'", ">");
    //H1
    follow.addFF("H1'", ">");
    //H2
    follow.addFF("H2'", ">");
    //H3
    follow.addFF("H3'", ">");
    //texto
    follow.addFF("texto'", ">");
    follow.addFF("texto'", "-#");
    follow.addFF("texto'", "-##");
    follow.addFF("texto'", "-###");
    follow.addFF("texto'", "-*");
    follow.addFF("texto'", "-/");
    follow.addFF("texto'", "-_");
    follow.addFF("texto'", "-~");
    follow.addFF("texto'", "-!");
    follow.addFF("texto'", "*/");
    follow.addFF("texto'", ":/");
    follow.addFF("texto'", ".png/");
    follow.addFF("texto'", ".jpg/");
    follow.addFF("texto'", "-}");
    follow.addFF("texto'", "-|");
    //estilo
    follow.addFF("estilo'", ">");
    follow.addFF("estilo'", "-#");
    follow.addFF("estilo'", "-##");
    follow.addFF("estilo'", "-###");
    follow.addFF("estilo'", "LETRAS");
    follow.addFF("estilo'", "*-");
    follow.addFF("estilo'", "/-");
    follow.addFF("estilo'", "_-");
    follow.addFF("estilo'", "~-");
    follow.addFF("estilo'", "-*");
    follow.addFF("estilo'", "-/");
    follow.addFF("estilo'", "-_");
    follow.addFF("estilo'", "-~");
    follow.addFF("estilo'", "-!");
    follow.addFF("estilo'", "*/");
    follow.addFF("estilo'", ":/");
    follow.addFF("estilo'", ".png/");
    follow.addFF("estilo'", ".jpg/");
    follow.addFF("estilo'", "-}");
    follow.addFF("estilo'", "-|");
    //bold
    follow.addFF("bold'", ">");
    follow.addFF("bold'", "-#");
    follow.addFF("bold'", "-##");
    follow.addFF("bold'", "-###");
    follow.addFF("bold'", "LETRAS");
    follow.addFF("bold'", "*-");
    follow.addFF("bold'", "/-");
    follow.addFF("bold'", "_-");
    follow.addFF("bold'", "~-");
    follow.addFF("bold'", "-*");
    follow.addFF("bold'", "-/");
    follow.addFF("bold'", "-_");
    follow.addFF("bold'", "-~");
    follow.addFF("bold'", "-!");
    follow.addFF("bold'", "*/");
    follow.addFF("bold'", ":/");
    follow.addFF("bold'", ".png/");
    follow.addFF("bold'", ".jpg/");
    follow.addFF("bold'", "-}");
    follow.addFF("bold'", "-|");
    //italic
    follow.addFF("italic'", ">");
    follow.addFF("italic'", "-#");
    follow.addFF("italic'", "-##");
    follow.addFF("italic'", "-###");
    follow.addFF("italic'", "LETRAS");
    follow.addFF("italic'", "*-");
    follow.addFF("italic'", "/-");
    follow.addFF("italic'", "_-");
    follow.addFF("italic'", "~-");
    follow.addFF("italic'", "-*");
    follow.addFF("italic'", "-/");
    follow.addFF("italic'", "-_");
    follow.addFF("italic'", "-~");
    follow.addFF("italic'", "-!");
    follow.addFF("italic'", "*/");
    follow.addFF("italic'", ":/");
    follow.addFF("italic'", ".png/");
    follow.addFF("italic'", ".jpg/");
    follow.addFF("italic'", "-}");
    follow.addFF("italic'", "-|");
    //underline
    follow.addFF("underline'", ">");
    follow.addFF("underline'", "-#");
    follow.addFF("underline'", "-##");
    follow.addFF("underline'", "-###");
    follow.addFF("underline'", "LETRAS");
    follow.addFF("underline'", "*-");
    follow.addFF("underline'", "/-");
    follow.addFF("underline'", "_-");
    follow.addFF("underline'", "~-");
    follow.addFF("underline'", "-*");
    follow.addFF("underline'", "-/");
    follow.addFF("underline'", "-_");
    follow.addFF("underline'", "-~");
    follow.addFF("underline'", "-!");
    follow.addFF("underline'", "*/");
    follow.addFF("underline'", ":/");
    follow.addFF("underline'", ".png/");
    follow.addFF("underline'", ".jpg/");
    follow.addFF("underline'", "-}");
    follow.addFF("underline'", "-|");
    //strikethrough
    follow.addFF("strikethrough'", ">");
    follow.addFF("strikethrough'", "-#");
    follow.addFF("strikethrough'", "-##");
    follow.addFF("strikethrough'", "-###");
    follow.addFF("strikethrough'", "LETRAS");
    follow.addFF("strikethrough'", "*-");
    follow.addFF("strikethrough'", "/-");
    follow.addFF("strikethrough'", "_-");
    follow.addFF("strikethrough'", "~-");
    follow.addFF("strikethrough'", "-*");
    follow.addFF("strikethrough'", "-/");
    follow.addFF("strikethrough'", "-_");
    follow.addFF("strikethrough'", "-~");
    follow.addFF("strikethrough'", "-!");
    follow.addFF("strikethrough'", "*/");
    follow.addFF("strikethrough'", ":/");
    follow.addFF("strikethrough'", ".png/");
    follow.addFF("strikethrough'", ".jpg/");
    follow.addFF("strikethrough'", "-}");
    follow.addFF("strikethrough'", "-|");
    //lista
    follow.addFF("lista", ">");
    //listaP
    follow.addFF("listaP", ">");
    //item
    follow.addFF("item", "!-");
    follow.addFF("item", ">");
    //comentario
    follow.addFF("comentario", ">");
    //url
    follow.addFF("url", ">");
    //imagenes
    follow.addFF("imagenes", ">");
    //finImg
    follow.addFF("finImg", ">");
    //tabla
    follow.addFF("tabla", ">");
    //tablaP
    follow.addFF("tablaP", ">");
    //fila
    follow.addFF("fila", "|-");
    follow.addFF("fila", ">");
    //filaP
    follow.addFF("filaP", "|-");
    follow.addFF("filaP", ">");
    //celda
    follow.addFF("celda", "|-");
    follow.addFF("celda", ">");
    follow.addFF("celda", "&");
    //head
    follow.addFF("head", "|-");
    //headP
    follow.addFF("headP", "|-");
    //hd
    follow.addFF("hd", "|-");
    follow.addFF("hd", ">");
    follow.addFF("hd", "%");

}


//SCANNER
char get_char(string line) {
    if (pos < line.size()) {
        char aux = line[pos];
        pos++;
        // cout << pos << " " << aux << endl;
        return aux;
    }
    return EOF;
}
char peek_char(string line) { return line[pos]; }
Token get_token(string line, int id_linea) {
    char aux = get_char(line);

    if (pos <= line.size())
    {
        // BLOQUES
        if (aux == '>') // End bloque
            return Token{ "E_Bloque", ">", id_linea, pos };
        if (aux == '<') // Begin bloque
            return Token{ "B_Bloque", "<", id_linea, pos };

        // BEGIN HEADER
        if (aux == '#') // Begin bold
        {
            if (peek_char(line) == '#') {
                get_char(line);
                if (peek_char(line) == '#') {
                    get_char(line);
                    pos++;
                    return Token{ "B_h3", "###-", id_linea, pos };
                }
                else {
                    pos++;
                    return Token{ "B_h2", "##-", id_linea, pos };
                }
            }
            else {
                pos++;
                return Token{ "B_h1", "#-", id_linea, pos };
            }
        }

        // END Headers -# -## -###, bold -*, italic -/, underline -_, strikethrough
        // -~, item -!, celda -|
        else if (aux == '-') {
            if (peek_char(line) == '#') {
                get_char(line);
                if (peek_char(line) == '#') {
                    get_char(line);
                    if (peek_char(line) == '#') {
                        get_char(line);
                        return Token{ "E_h3", "-###", id_linea, pos };
                    }
                    return Token{ "E_h2", "-##", id_linea, pos };
                }
                return Token{ "E_h1", "-#", id_linea, pos };
            }
            // END ESTILOS
            if (peek_char(line) == '*') // End bold
            {
                get_char(line);
                return Token{ "E_bold", "-*", id_linea, pos };
            }
            else if (peek_char(line) == '/') // End italic
            {
                get_char(line);
                return Token{ "E_italic", "-/", id_linea, pos };
            }
            else if (peek_char(line) == '_') // End underline
            {
                get_char(line);
                return Token{ "E_underline", "-_", id_linea, pos };
            }
            else if (peek_char(line) == '~') // End strikethrough
            {
                get_char(line);
                return Token{ "E_strikethrough", "-~", id_linea, pos };
            }
            // END ITEM
            else if (peek_char(line) == '!') // End item
            {
                get_char(line);
                return Token{ "E_item", "-!", id_linea, pos };
            }
            // END CELDA
            else if (peek_char(line) == '|') // End celda
            {
                get_char(line);
                /*if (line[pos] == '>') {
                    return Token{ "E_fila", "-|", id_linea, pos };
                }*/
                return Token{ "E_celda", "-|", id_linea, pos };
            }
            else if (peek_char(line) == '}') // End hd
            {
                get_char(line);
                return Token{ "E_hd", "-}", id_linea, pos };
            }
        }
        // BEGIN bold *-, END comentario */
        else if (aux == '*') // Begin bold
        {
            if (peek_char(line) == '-') {
                get_char(line);
                return Token{ "B_bold", "*-", id_linea, pos };
            }
            if (peek_char(line) == '/') // End Comentario
            {
                get_char(line);
                return Token{ "E_comentario", "*/", id_linea, pos };
            }
        }
        // BEGIN: italic /-, Comentario /*, url /url:, img /img:
        else if (aux == '/') {
            if (peek_char(line) == '-') // Begin italic
            {
                get_char(line);
                return Token{ "B_italic", "/-", id_linea, pos };
            }
            if (peek_char(line) == '*') // Begin Comentario
            {
                get_char(line);
                return Token{ "B_comentario", "/*", id_linea, pos };
            }
            if (peek_char(line) == 'u') // Begin url
            {
                int aux_pos = pos;
                pos++;
                if (peek_char(line) == 'r') {
                    pos++;
                    if (peek_char(line) == 'l') {
                        pos++;
                        if (peek_char(line) == ':') {
                            get_char(line);
                            return Token{ "B_url", "/url:", id_linea, pos };
                        }
                    }
                }
                pos = aux_pos;
            }
            if (peek_char(line) == 'i') // Begin img
            {
                int aux_pos = pos;
                pos++;
                if (peek_char(line) == 'm') {
                    pos++;
                    if (peek_char(line) == 'g') {
                        pos++;
                        if (peek_char(line) == ':') {
                            get_char(line);
                            return Token{ "B_img", "/img:", id_linea, pos };
                        }
                    }
                }
                pos = aux_pos;
            }
        }
        // Begin underline _-
        else if (aux == '_') {
            if (peek_char(line) == '-') {
                get_char(line);
                return Token{ "B_underline", "_-", id_linea, pos };
            }
        }
        // Begin strikethrough ~-
        else if (aux == '~') {
            if (peek_char(line) == '-') {
                get_char(line);
                return Token{ "B_strikethrough", "~-", id_linea, pos };
            }
        }
        // BEGIN ITEM !-
        else if (aux == '!') // Begin item
        {
            if (peek_char(line) == '-') {
                get_char(line);
                return Token{ "B_item", "!-", id_linea, pos };
            }
        }
        // BEGIN CELDA |-
        else if (aux == '|') // Begin celda
        {
            if (peek_char(line) == '-') {
                get_char(line);
                /*if (line[pos - 3] == '}') {
                    return Token{ "B_fila", "|-", id_linea, pos };
                }*/
                return Token{ "B_celda", "|-", id_linea, pos };
            }
        }
        // Begin head
        else if (aux == '{')
        {
            if (peek_char(line) == '-') {
                get_char(line);
                return Token{ "B_hd", "{-", id_linea, pos };
            }
        }
        // END URL :/
        else if (aux == ':') // End url
        {
            if (peek_char(line) == '/') {
                get_char(line);
                return Token{ "E_url", ":/", id_linea, pos };
            }
        }
        // END IMG  ->  .png/, .jpg/
        else if (aux == '.') //
        {
            if (peek_char(line) == 'p') // End png
            {
                int aux_pos = pos;
                pos++;
                if (peek_char(line) == 'n') {
                    pos++;
                    if (peek_char(line) == 'g') {
                        pos++;
                        if (peek_char(line) == '/') {
                            get_char(line);
                            return Token{ "E_png", ".png/", id_linea, pos };
                        }
                    }
                }
                pos = aux_pos;
            }
            else if (peek_char(line) == 'j') // End png
            {
                int aux_pos = pos;
                pos++;
                if (peek_char(line) == 'p') {
                    pos++;
                    if (peek_char(line) == 'g') {
                        pos++;
                        if (peek_char(line) == '/') {
                            get_char(line);
                            return Token{ "E_jpg", ".jpg/", id_linea, pos };
                        }
                    }
                }
                pos = aux_pos;
            }
        }

        //  & 
        else if (aux == '&') {
            return Token{ "Add_Fila", "&", id_linea, pos };
        }

        //  % 
        else if (aux == '%') {
            get_char(line);
            return Token{ "Add_Head", "%", id_linea, pos };
        }

        // LETRAS
        if (isalnum(aux) || isspace(aux)) {
            string value = "";
            value += aux;
            while (isalnum(peek_char(line)) || isspace(peek_char(line)) || ispunct(peek_char(line))) {
                char next_char = peek_char(line);
                if (next_char == '<' || next_char == '>' || next_char == '#' || next_char == '*' || next_char == '/' || next_char == '-' || next_char == '!' || next_char == '|' || next_char == ':' || next_char == '.' ) {
                    if (next_char == '.') {
                        int pos_aux = pos;
                        if (line[pos_aux+1] == 'p') {
                            if (line[pos_aux + 2] == 'n') {
                                if (line[pos_aux + 3] == 'g') {
                                    cout << "VALOR: " << value << endl;
                                    return Token{ "texto","LETRAS", id_linea, pos, value };
                                }
                            }
                        }
                        else if (line[pos_aux + 1] == 'j') {
                            if (line[pos_aux + 2] == 'p') {
                                if (line[pos_aux + 3] == 'g') {
                                    cout << "VALOR: " << value << endl;
                                    return Token{ "texto","LETRAS", id_linea, pos, value };
                                }
                            }
                        }
                        value += get_char(line);
                    }
                    else {
                        break;
                    }
                }
                value += get_char(line);
            }
            cout << "VALOR: " << value << endl;
            return Token{ "texto","LETRAS", id_linea, pos, value };
        }
        //else
            //return Token{ "invalido", to_string(aux), id_linea, pos };
    }

    return Token{ "Fin", "$", id_linea, pos };
}

stack<Token> scanner(string line, int id_linea) {
    stack<Token> tokens;
    pos = 0;
    //tokens.push(Token{"init", "$", id_linea, pos});
    tokens.push(get_token(line, id_linea));
    while (tokens.top().token_type != "Fin") {
        tokens.push(get_token(line, id_linea));
    }
    return tokens;
}
void print_stack(stack<Token> tokens) {
    while (!tokens.empty()) {
        Token token = tokens.top();
        token.print_token();
        tokens.pop();
    }
}

void translate_to_markdown(const vector<Token>& tokens, const string& filename) {
    ofstream markdown(filename, std::ios::app); // Abrir en modo de adición
    if (!markdown.is_open()) {
        cerr << "Error al abrir el archivo " << filename << " para escribir." << endl;
        return;
    }

    for (const auto& token : tokens) {
        if (token.token_type == "B_h1") {
            markdown << "# ";
        }
        else if (token.token_type == "B_h2") {
            markdown << "## ";
        }
        else if (token.token_type == "B_h3") {
            markdown << "### ";
        }
        else if (token.token_type == "E_h1" || token.token_type == "E_h2" || token.token_type == "E_h3") {
            markdown << endl;
        }
        else if (token.token_type == "B_bold") {
            markdown << "**";
        }
        else if (token.token_type == "E_bold") {
            markdown << "**";
        }
        else if (token.token_type == "B_italic") {
            markdown << "*";
        }
        else if (token.token_type == "E_italic") {
            markdown << "*";
        }
        else if (token.token_type == "B_underline") {
            markdown << "<u>";
        }
        else if (token.token_type == "E_underline") {
            markdown << "</u>";
        }
        else if (token.token_type == "B_strikethrough") {
            markdown << "~~";
        }
        else if (token.token_type == "E_strikethrough") {
            markdown << "~~";
        }
        else if (token.token_type == "B_item") {
            markdown << "- ";
        }
        else if (token.token_type == "E_item") {
            markdown << endl;
        }
        else if (token.token_type == "B_celda") {
            markdown << "| ";
        }
        else if (token.token_type == "E_celda") {
            markdown << " |" << endl;
        }
        else if (token.token_type == "B_hd") {
            markdown << "| ";
        }
        else if (token.token_type == "E_hd") {
            markdown << " |" << endl;
            markdown << "|:------------------------:|" << endl;
        }
        else if (token.token_type == "B_fila") {
            markdown << "| ";
        }
        else if (token.token_type == "E_fila") {
            markdown << " |";
        }
        else if (token.token_type == "Add_Head") {
            continue;
        }
        else if (token.token_type == "E_Bloque") {
            markdown << endl;
            markdown << endl;
        }
        else if (token.token_type == "B_Bloque") {
            continue;
        }
        else if (token.token_type == "texto") {
            markdown << token.aux_value;
        }
        else if (token.token_type == "B_img") {
            markdown << "![imagen](";
        }
        else if (token.token_type == "E_png") {
            markdown << ".png)";
            markdown << endl;
        }
        else if (token.token_type == "E_jpg") {
            markdown << ".jpg)";
            markdown << endl;
        }
        else if (token.token_type == "B_url") {
            markdown << "<";
        }
        else if (token.token_type == "E_url") {
            markdown << ">";
            markdown << endl;
        }
        //<http://www.limni.net>
        //![Texto alternativo](/ruta/a/la/imagen.jpg)
    }
    markdown.close();
    cout << "Archivo " << filename << " generado exitosamente." << endl;
}


//PARSER
bool parse(stack<Token>& tokens, const LL1ParserTable& parserTable, const FF& first, const FF& follow, vector<Token>& error_list, vector<Token>& parsed_tokens) {
    stack<string> parsing_stack;
    parsing_stack.push("$");
    parsing_stack.push("documento");

    stack<Token> inputTokens;

    // Voltea la pila de tokens
    while (!tokens.empty()) {
        inputTokens.push(tokens.top());
        tokens.pop();
    }
    tokens = inputTokens;

    while (!tokens.empty() && parsing_stack.top() != "$") {
        Token current_token = tokens.top();
        string TOS = parsing_stack.top();
        cout << TOS << " " << current_token.token_value << endl;

        //|| (TOS == "texto" && current_token.token_type == "texto")
        if ((TOS == current_token.token_value)) {  // Coinciden los terminales
            parsing_stack.pop();
            parsed_tokens.push_back(current_token);  // Añadido: almacena tokens procesados correctamente
            /*cout << "Parsed tokens" << endl;
            for (int i = 0; i < parsed_tokens.size(); i++) {
                cout << parsed_tokens[i].token_value << " ";
            }
            cout << endl << " Fin " << endl;*/
            tokens.pop();
        }
        else if (TOS == "$") {
            if (current_token.token_value == "Fin") {
                return true;
            }
            else {
                return false;
            }
        }
        else if (TOS == "epsilon") {
            parsing_stack.pop();
        }
        else {
            // Busca la producción en la tabla LL(1)
            auto it = parserTable.table.find(make_pair(TOS, current_token.token_value));
            if (it != parserTable.table.end()) {
                Production production = it->second;
                parsing_stack.pop();
                for (auto prod_it = production.production.rbegin(); prod_it != production.production.rend(); ++prod_it) {
                    parsing_stack.push(*prod_it);
                }
            }
            else {
                // Error de sintaxis
                error_list.push_back(current_token);
                bool recovered = false;

                if (first.FF_table.find(TOS) != first.FF_table.end()) {
                    string firstSet = first.FF_table.at(TOS);
                    if (firstSet.find(current_token.token_value) != string::npos) {
                        recovered = true;
                    }
                }

                if (!recovered && follow.FF_table.find(TOS) != follow.FF_table.end()) {
                    string followSet = follow.FF_table.at(TOS);
                    if (followSet.find(current_token.token_value) != string::npos) {
                        parsing_stack.pop();
                        recovered = true;
                    }
                }

                if (!recovered) {
                    tokens.pop();
                }
            }
        }
    }

    return parsing_stack.top() == "$" && tokens.top().token_type == "Fin";
}


string Archivo(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        ofstream outfile(filename);
        outfile << "<##-*-~-texto-~-*-##>"; // Contenido predeterminado
        outfile.close();
        infile.open(filename);
    }
    string content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    infile.close();
    return content;
}

void generate_pdf(const string& md_filename, const string& pdf_filename) {
    string command = "pandoc " + md_filename + " -o " + pdf_filename;
    system(command.c_str());
}

void processLine(const string& line, int line_number, const string& md, const string& pdf) {
    stack<Token> tokens_leidos = scanner(line, line_number);
    print_stack(tokens_leidos);

    LL1ParserTable parserTable;
    AddRules(parserTable);

    FF first;
    AddFirst(first);
    FF follow;
    AddFollow(follow);

    vector<Token> error_list;
    vector<Token> parsed_tokens;  // Añadido: vector para tokens procesados
    bool es_valida = parse(tokens_leidos, parserTable, first, follow, error_list, parsed_tokens);

    if (es_valida && error_list.size() == 0) {
        cout << "La entrada es valida." << endl;
        translate_to_markdown(parsed_tokens, md);  // Genera el Markdown
    }
    else {
        cout << "La entrada es invalida." << endl;
        for (const auto& error : error_list) {
            cout << "Error: Token inesperado " << error.token_value << " en linea " << error.line << ", columna " << error.column << endl;
            cout << endl;
        }
    }
}



int main() {
    string filename = "input.txt";
    string markfile = "output.md";
    string pdfile = "generado.pdf";
    string content = Archivo(filename);

    ifstream infile(filename);
    string line;
    int line_number = 1;
    while (getline(infile, line)) {
        cout << "Procesando linea " << line_number << ": " << line << endl;
        processLine(line, line_number, markfile, pdfile);
        line_number++;
    }
    infile.close();
    generate_pdf(markfile, pdfile);
    return 0;
}

