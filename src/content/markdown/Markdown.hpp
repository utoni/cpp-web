#ifndef MARKDOWN_H
#define MARKDOWN_H 1

#include "../../Content.hpp"
#include "../../Filesystem.hpp"

using Markdowns = std::unordered_map<std::string, std::shared_ptr<std::string> >;

class Markdown : public Content
{
public:
    explicit Markdown(std::string uriBasePath, std::string markdownFilesPath, std::string mainTemplatePath = "");

    bool Init(std::string const & uriBasePath);
    bool Init() override;
    void Shutdown() override;
    bool Render(RequestResponse & rr, RenderData & rd, std::string & out) override;

    std::string & GetUriBasePath() override;
    std::string const & GetMainTemplate() const override;
    Redirections & GetRedirections() override;

    bool HasMarkdownFile(std::string filePath) const;
    bool HasMarkdownURI(std::string uriPath) const;
    Markdowns & GetMarkdowns();
    std::shared_ptr<std::string> const & GetMarkdownHTML(std::string uriPath);

private:
    std::string m_UriBasePath;
    std::string m_MainTemplatePath;
    std::string m_MarkdownFilesPath;
    Redirections m_Redirections;
    Markdowns m_Markdowns;
};

#endif
