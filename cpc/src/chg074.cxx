
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

#include <xalanc/PlatformSupport/XSLException.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>

#include "challenge.h"

XERCES_CPP_NAMESPACE_USE

namespace xml = XERCES_CPP_NAMESPACE;
namespace xsl = xalanc;

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

struct resRelease {
    template <class T>
    void operator()(T* ptr) {
        ptr->release();
    }
};


void loadMovieData() {
    auto xmlImpl = xml::DOMImplementationRegistry::getDOMImplementation(ChToXCh{"LS"});
    auto parser = std::unique_ptr<xml::DOMLSParser, resRelease>(((xml::DOMImplementationLS*)xmlImpl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0));
    DefaultErrorHandler handler;
    if (parser->getDomConfig()->canSetParameter(xml::XMLUni::fgDOMValidate, false))
        parser->getDomConfig()->setParameter(xml::XMLUni::fgDOMValidate, false);
    parser->getDomConfig()->setParameter(xml::XMLUni::fgDOMErrorHandler, &handler);

    try {
        auto doc = parser->parseURI("../data/movies.xml");
        if (doc != nullptr) {
            xsl::XercesParserLiaison parserLiaison;
            xsl::XercesDOMSupport domSupport{parserLiaison};
            xsl::XercesDOMWrapperParsedSource parsedSource{doc, parserLiaison, domSupport};
            auto xslDoc = parsedSource.getDocument();
            xsl::XPathEvaluator evaluator;
            xsl::XalanDocumentPrefixResolver prefResolver{xslDoc};
            auto ctx = evaluator.selectSingleNode(domSupport, xslDoc, xsl::XalanDOMString("/").c_str(), prefResolver);
            if (ctx == nullptr) {
                std::cerr << "No nodes matched the location path: /movies" << std::endl;
            } else {
                // found
                xsl::XObjectPtr result{
                    //evaluator.evaluate(domSupport, ctx, xsl::XalanDOMString{"/movies/movie[@year>1995]"}.c_str(), prefResolver)
                    evaluator.evaluate(domSupport, ctx, xsl::XalanDOMString{"/movies/movie/cast/role[last()]"}.c_str(), prefResolver)
                };
                auto const& nodeset = result->nodeset();
                for (size_t i = 0u, end = nodeset.getLength(); i < end; ++i) {
                    auto const attr = nodeset.item(i)->getAttributes();
                    //auto name = attr->getNamedItem(xsl::XalanDOMString{"title"});
                    auto name = attr->getNamedItem(xsl::XalanDOMString{"star"});
                    if (name != nullptr) {
                        auto namestr = name->getNodeValue();
                        auto n = xsl::TranscodeToLocalCodePage(namestr);

                        std::cout << "name: " << n << std::endl;
                    }
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
}

void test0() {
    try {
        xml::XMLPlatformUtils::Initialize();
        xsl::XPathEvaluator::initialize();

        loadMovieData();
    } catch (xml::XMLException const& ex) {
        std::cerr << "Error in xerces init: " << XChToCh{ex.getMessage()}  << std::endl;
    } catch (xsl::XSLException const& ex) {
        xsl::XalanDOMString errtext;
        std::cerr << "Error in xalan init: " << ex.defaultFormat(errtext) << std::endl;
    }

    xsl::XPathEvaluator::terminate();
    xml::XMLPlatformUtils::Terminate();

}
int main(int, char**) {
    test0();

    return 0;
}