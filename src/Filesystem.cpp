#include "Filesystem.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

static std::string make_path_relative(std::string &path, std::string &root) {
  return std::filesystem::relative(path, root);
}

bool Filesystem::AddSingleFile(std::string path, std::string root) {
  std::ifstream ifs(path, std::ios::binary | std::ios::ate);

  if (!ifs) {
    return false;
  }

  auto end = ifs.tellg();
  if (end <= 0) {
    return false;
  }
  if (!ifs.seekg(0, std::ios::beg)) {
    return false;
  }

  auto size = std::size_t(end - ifs.tellg());

  if (size == 0) {
    return false;
  }

  struct file_data fd = {};
  try {
    fd.data.reserve(size);
  } catch (const std::exception &e) {
    return false;
  }

  if (!ifs.read((char *)fd.data.data(), fd.data.size())) {
    return false;
  }

  std::string relpath = make_path_relative(path, root);
  if (m_Files.count(relpath) > 0) {
    std::cout << "Adding file: " << path << " and overwriting " << relpath
              << std::endl;
  } else {
    std::cout << "Adding file: " << path << " as " << relpath << std::endl;
  }

  std::string ext = std::filesystem::path(relpath).extension();
  if (ext == ".html" || ext == ".tmpl")
  {
    std::string tmpl(fd.data.data(), fd.data.data() + fd.data.size());
    m_Templates[relpath] = inja::Template(tmpl);
    std::cout << "File: " << relpath << " may contain a renderable template." << std::endl;
  } else {
    m_Files[relpath] = fd;
  }

  return true;
}

bool Filesystem::Scan(std::string root) {
  for (const auto &entry : std::filesystem::directory_iterator(root)) {
    AddSingleFile(entry.path(), root);
  }
  return true;
}

void Filesystem::AddInjaCallback(std::string functionName, std::size_t numberOfArgs, inja::CallbackFunction function)
{
  m_Inja.add_callback(functionName, numberOfArgs, function);
}

void Filesystem::AddVoidInjaCallback(std::string functionName, std::size_t numberOfArgs, inja::VoidCallbackFunction function)
{
  m_Inja.add_void_callback(functionName, numberOfArgs, function);
}
