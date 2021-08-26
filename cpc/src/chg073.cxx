#include <vector>
#include <string_view>
#include <iostream>
#include <memory>
#include <charconv>
#include <array>
#include <ranges>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "challenge.h"
#include "movie_data.h"

XERCES_CPP_NAMESPACE_USE

namespace xml = XERCES_CPP_NAMESPACE;

struct XChToCh {
    XChToCh(XMLCh const* const toTrans)
    : text{xml::XMLString::transcode(toTrans)} {
    }
    ~XChToCh() {
        if (text != nullptr)
            xml::XMLString::release(&text);
    }
    operator char const*() const {
        return text;
    }

private:
    char* text;
};

struct ChToXCh {
    ChToXCh(char const* const toTrans)
    : text{xml::XMLString::transcode(toTrans)} {
    }
    ~ChToXCh() {
        if (text) {
            xml::XMLString::release(&text);
        }
    }
    operator XMLCh const*() const {
        return text;
    }

private:
    XMLCh* text;
};

struct DefaultErrorHandler : public xml::DOMErrorHandler {
    bool handleError(DOMError const& err) {
        auto loc = err.getLocation();
        std::cerr << "Error["
            << (uint64_t)loc->getLineNumber() << ","
            << (uint64_t)loc->getColumnNumber() << "] "
            << XChToCh{err.getMessage()}
            << std::endl;;
        return err.getSeverity() != xml::DOMError::DOM_SEVERITY_FATAL_ERROR;
    }
};

struct ParserErrorHandler : public xml::ErrorHandler {
    virtual void error(SAXParseException const& exc) override {
        std::cerr << "Error:["
            << (char const*)exc.getLineNumber() << ","
            << (char const*)exc.getColumnNumber() << "] "
            << XChToCh{exc.getMessage()};
    }

    virtual void fatalError(xml::SAXParseException const& exc) override {
        std::cerr << "Fatal:["
            << (char const*)exc.getLineNumber() << ","
            << (char const*)exc.getColumnNumber() << "] "
            << XChToCh{exc.getMessage()};
    }

    virtual void resetErrors() override {

    }

    virtual void warning(xml::SAXParseException const& exc) override {
        std::cerr << "Warning:["
            << (char const*)exc.getLineNumber() << ","
            << (char const*)exc.getColumnNumber() << "] "
            << XChToCh{exc.getMessage()};
    }
};

struct implDeletor {
    void operator()(xml::DOMImplementation*) {
        // do nothing on application code
    }
};
void serializeXml(std::unique_ptr<xml::DOMDocument> const& document , std::string_view filepath) {
    auto xmlImpl = xml::DOMImplementationRegistry::getDOMImplementation(ChToXCh{"LS"});
    if (!xmlImpl) {
        std::cerr << "Cannot get DOM(LS) implementation" << std::endl;
        return;
    }
    try {
        auto writer = xmlImpl->createLSSerializer();
        if (writer->getDomConfig()->canSetParameter(xml::XMLUni::fgDOMWRTFormatPrettyPrint, true))
            writer->getDomConfig()->setParameter(xml::XMLUni::fgDOMWRTFormatPrettyPrint, true);
        writer->setNewLine(ChToXCh{"\n"});

        auto fpath = ChToXCh{filepath.data()};
        auto fmt = std::make_unique<xml::LocalFileFormatTarget>(fpath);
        auto output = xmlImpl->createLSOutput();
        output->setByteStream(fmt.get());
        writer->write(document.get(), output);

        writer->release();
        output->release();
    } catch (DOMException const& ex) {

    }
}

void serializeMovies(cpc::movie_list const& movies, std::string_view filepath) {
    auto xmlImpl = xml::DOMImplementationRegistry::getDOMImplementation(ChToXCh{"Core"});
    if (!xmlImpl) {
        std::cerr << "Cannot get DOM(Core) implementation" << std::endl;
        return;
    }
    std::array<char, 100> numbuf;
    auto nbbegin = std::ranges::begin(numbuf);
    auto nbend = std::ranges::end(numbuf);
    try {
        auto document = std::unique_ptr<xml::DOMDocument>(xmlImpl->createDocument(nullptr, ChToXCh{"movies"}, nullptr));
        if (document) {
            auto rt = document->getDocumentElement();
            for (auto const& mov : movies) {
                auto movieElem = document->createElement(ChToXCh{"movie"});
                if (auto [idptr, ec] = std::to_chars(nbbegin, nbend, mov.id); ec == std::errc{}) {
                    *idptr = (char)0;
                    movieElem->setAttribute(ChToXCh{"id"}, ChToXCh{numbuf.data()});
                }
                if (auto [ptr, ec] = std::to_chars(nbbegin, nbend, mov.year); ec == std::errc{}) {
                    *ptr = (char)0;
                    movieElem->setAttribute(ChToXCh{"year"}, ChToXCh{numbuf.data()});
                }
                if (auto [ptr, ec] = std::to_chars(nbbegin, nbend, mov.length); ec == std::errc{}) {
                    *ptr = (char)0;
                    movieElem->setAttribute(ChToXCh{"length"}, ChToXCh{numbuf.data()});
                }
                movieElem->setAttribute(ChToXCh{"title"}, ChToXCh{mov.title.c_str()});
                rt->appendChild(movieElem);

                auto castElem = document->createElement(ChToXCh{"cast"});
                movieElem->appendChild(castElem);
                for (auto const& cast : mov.cast) {
                    auto roleElem = document->createElement(ChToXCh{"role"});
                    roleElem->setAttribute(ChToXCh{"star"}, ChToXCh{cast.actor.c_str()});
                    roleElem->setAttribute(ChToXCh{"name"}, ChToXCh{cast.role.c_str()});
                    castElem->appendChild(roleElem);
                }

                auto directors = document->createElement(ChToXCh{"directors"});
                movieElem->appendChild(directors);
                for (auto const& director : mov.directors) {
                    auto dirElem = document->createElement(ChToXCh{"director"});
                    dirElem->setAttribute(ChToXCh{"name"}, ChToXCh{director.c_str()});
                    directors->appendChild(dirElem);
                }

                auto writers = document->createElement(ChToXCh{"writers"});
                movieElem->appendChild(writers);
                for (auto const& w : mov.writers) {
                    auto writerElem = document->createElement(ChToXCh{"writer"});
                    writerElem->setAttribute(ChToXCh{"name"}, ChToXCh{w.c_str()});
                    writers->appendChild(writerElem);
                }
            }

            serializeXml(document, filepath);
        } else {
            std::cerr << "Document creation failed." << std::endl;
        }

    } catch (DOMException const& ex) {

    }
}

struct resRelease {
    template <class T>
    void operator()(T* ptr) {
        ptr->release();
    }
};
cpc::movie_list deserialize(std::string_view filepath) {
    auto xmlImpl = xml::DOMImplementationRegistry::getDOMImplementation(ChToXCh{"LS"});
    auto parser = std::unique_ptr<xml::DOMLSParser, resRelease>(((xml::DOMImplementationLS*)xmlImpl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0));
    DefaultErrorHandler handler;
    if (parser->getDomConfig()->canSetParameter(xml::XMLUni::fgDOMValidate, false))
        parser->getDomConfig()->setParameter(xml::XMLUni::fgDOMValidate, false);
    parser->getDomConfig()->setParameter(xml::XMLUni::fgDOMErrorHandler, &handler);
    cpc::movie_list ret;

    try {
        char const* furi = filepath.data();
        auto doc = parser->parseURI(furi);
        if (doc != nullptr) {
            auto root = doc->getDocumentElement();
            auto movies = root->getElementsByTagName(ChToXCh{"movie"});
            if (movies != nullptr) {
                for (XMLSize_t i = 0u, end = movies->getLength(); i < end; ++i) {
                    unsigned int id, length;
                    int year;
                    auto mov = movies->item(i);
                    auto attr = mov->getAttributes();
                    auto idval = std::string(XChToCh{attr->getNamedItem(ChToXCh{"id"})->getNodeValue()});
                    auto titleval = std::string(XChToCh{attr->getNamedItem(ChToXCh{"title"})->getNodeValue()});
                    auto yearval = std::string(XChToCh{attr->getNamedItem(ChToXCh{"year"})->getNodeValue()});
                    auto lengthval = std::string(XChToCh{attr->getNamedItem(ChToXCh{"length"})->getNodeValue()});
                    if (auto [ptr, ec] = std::from_chars(idval.c_str(), idval.c_str() + idval.length(), id); ec != std::errc{}) {
                        std::cerr << "error, cannot convert to int value error: "  << idval << std::endl;
                    }
                    if (auto [ptr, ec] = std::from_chars(yearval.c_str(), yearval.c_str() + yearval.length(), year); ec != std::errc{}) {
                        std::cerr << "error, cannot convert to int value error: "  << yearval << std::endl;
                    }
                    if (auto [ptr, ec] = std::from_chars(lengthval.c_str(), lengthval.c_str() + lengthval.length(), length); ec != std::errc{}) {
                        std::cerr << "error, cannot convert to int value error: "  << lengthval << std::endl;
                    }

                    std::vector<cpc::casting_role> rolesData;
                    auto casts = ((xml::DOMElement*)mov)->getElementsByTagName(ChToXCh{"cast"});
                    auto cast = (xml::DOMElement*)casts->item(0);
                    auto roles = cast->getElementsByTagName(ChToXCh{"role"});
                    for (XMLSize_t i = 0u, end = roles->getLength(); i < end; ++i) {
                        auto role = roles->item(i);
                        auto roleAttr = role->getAttributes();
                        auto actor = std::string(XChToCh{roleAttr->getNamedItem(ChToXCh{"star"})->getNodeValue()});
                        auto name = std::string(XChToCh{roleAttr->getNamedItem(ChToXCh{"name"})->getNodeValue()});
                        rolesData.emplace_back(cpc::casting_role{actor, name});
                    }
                    std::vector<std::string> directorsData;
                    auto directors = static_cast<xml::DOMElement*>(((xml::DOMElement*)mov)->getElementsByTagName(ChToXCh{"directors"})->item(0));
                    auto directorList = directors->getElementsByTagName(ChToXCh{"director"});
                    for (XMLSize_t i = 0u, end = directorList->getLength(); i < end; ++i) {
                        auto director = directorList->item(i);
                        auto direcAttr = director->getAttributes();
                        auto name = std::string(XChToCh{direcAttr->getNamedItem(ChToXCh{"name"})->getNodeValue()});
                        directorsData.emplace_back(name);
                    }
                    std::vector<std::string> writersData;
                    auto writers = static_cast<xml::DOMElement*>(((xml::DOMElement*)mov)->getElementsByTagName(ChToXCh{"writers"})->item(0));
                    auto writersList = writers->getElementsByTagName(ChToXCh{"writer"});
                    for (XMLSize_t i = 0u, end = writersList->getLength(); i < end; ++i) {
                        auto writer = writersList->item(i);
                        auto writerAttr = writer->getAttributes();
                        auto name = std::string(XChToCh{writerAttr->getNamedItem(ChToXCh{"name"})->getNodeValue()});
                        writersData.emplace_back(name);
                    }
                    ret.emplace_back(cpc::movie{ id, titleval, year, length, rolesData, directorsData, writersData});
                }
            }
        }
    } catch (xml::XMLException const& exc) {
        std::cerr << "XMLException:"
            << XChToCh{exc.getMessage()} << std::endl;
    } catch (xml::DOMException const& exc) {
        std::cerr << "DOMException:"
            << XChToCh{exc.getMessage()} << std::endl;
    } catch (...) {
        std::cerr << "Unexpected exception" << std::endl;
    }

    return ret;
}

void test0() { 
    try {
        xml::XMLPlatformUtils::Initialize();

        //auto movies = cpc::make_movie_list();
        //serializeMovies(movies, "../data/movies.xml");

        auto des = deserialize("movies.xml");
        for (auto const& m : des) {
            m.print();
        }
    } catch (xml::XMLException const& ex) {
        std::cerr << "Error in xerces init: " << XChToCh{ex.getMessage()}  << std::endl;
        return;
    }

    xml::XMLPlatformUtils::Terminate();
}

int main(int, char**) {
    test0();

    return 0;
}