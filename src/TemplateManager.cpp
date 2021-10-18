#include "TemplateManager.hpp"

#include <filesystem>

TemplateManager::TemplateManager()
{
    AddInjaCallback("test_fn", 0, [](inja::Arguments & args) {
        (void)args;
        return "Just a test fn.";
    });
    AddInjaCallback("test_return_true", 0, [](inja::Arguments & args) {
        (void)args;
        return true;
    });
    /*
     * indent(input: str, width: int, first: bool, blank: bool);
     */
    AddInjaCallback("indent", 4, [](inja::Arguments & args) {
        std::stringstream stream(args.at(0)->get<std::string>());
        std::string line, out;
        bool is_first_line = true;
        while (std::getline(stream, line))
        {
            if (is_first_line == false || args.at(2)->get<bool>() == true)
            {
                if (line != "" || args.at(3)->get<bool>() == false)
                {
                    line.insert(0, args.at(1)->get<std::size_t>(), ' ');
                }
            }
            line += '\n';
            out += line;
            is_first_line = false;
        }
        return out;
    });
}

void TemplateManager::ParseTemplates(Filesystem & fs)
{
    for (auto & tpl : fs.GetFiles())
    {
        std::string tmpl(tpl.second.data.data(), tpl.second.data.data() + tpl.second.data.size());
        m_Templates[tpl.first] = m_Inja.parse(tmpl);
        std::cout << "File: " << tpl.first << " may contain a renderable template." << std::endl;
    }
}

void TemplateManager::AddInjaCallback(std::string functionName,
                                      std::size_t numberOfArgs,
                                      inja::CallbackFunction function)
{
    m_Inja.add_callback(functionName, numberOfArgs, function);
}

void TemplateManager::AddVoidInjaCallback(std::string functionName,
                                          std::size_t numberOfArgs,
                                          inja::VoidCallbackFunction function)
{
    m_Inja.add_void_callback(functionName, numberOfArgs, function);
}

bool TemplateManager::TemplateExists(std::string const & templatePath)
{
    return m_Templates.find(templatePath) != m_Templates.end();
}

bool TemplateManager::RenderTemplate(std::string const & templatePath, RenderData const & rd, std::string & out)
{
    if (TemplateExists(templatePath) == false)
    {
        return false;
    }

    try
    {
        out = m_Inja.render(m_Templates[templatePath].content, rd);
    }
    catch (inja::RenderError & re)
    {
        std::cerr << "Render Error: " << re.what() << std::endl;
        return false;
    }

    return true;
}
