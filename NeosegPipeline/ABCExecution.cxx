#include "ABCExecution.h"

ABCExecution::ABCExecution(QString module)
    : Script(module)
{

}

void ABCExecution::update(){

    initialize();
    writeXMLFile();
    implementStop();
    implementCheckFileExistence();
    implementExecute();
    implementRun();
    writeScript();
}

void ABCExecution::initialize(){
    definePython();
    importGeneralModules();
    importXmlModules();

    defineExecutable("ABC");


    m_script += "logger = logging.getLogger('ABCPipeline')\n\n";

    m_script += "runningProcess = None\n\n";
}

void ABCExecution::implementRun(){

    m_script += "def run():\n";

    m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
    m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

    m_script += m_indent + "logger.info('=== ABC Execution ===')\n";


    m_script+= m_indent + "args = [ABC, parameters_path]\n";
    m_script+= m_indent + "execute(args)";
}

void ABCExecution::writeXMLFile(){

    QString XML_path = m_module_dir->filePath("parameters.xml");
    m_script += "parameters_path = \"" + XML_path + "\"\n";

    QFile* file = new::QFile(XML_path);
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
    stream->setAutoFormatting(true);

    stream->writeStartDocument();
    stream->writeDTD("<!DOCTYPE SEGMENTATION-PARAMETERS>");

    stream->writeStartElement("SEGMENTATION-PARAMETERS");


    writeABCParameters(stream);

    stream->writeEndElement();
    stream->writeEndDocument();

    file->close();

}

void ABCExecution::writeABCParameters(QXmlStreamWriter *stream){

    stream->writeTextElement(QString("SUFFIX"), m_parameters->getSuffix());

    stream->writeTextElement(QString("ATLAS-DIRECTORY"), m_parameters->getExistingAtlas());

    stream->writeTextElement(QString("ATLAS-ORIENTATION"), QString("file"));
    stream->writeTextElement(QString("ATLAS-FORMAT"), m_parameters->getAtlasFormat());
    stream->writeTextElement(QString("OUTPUT-DIRECTORY"), m_parameters->getOutput());
    stream->writeTextElement(QString("OUTPUT-FORMAT"), m_parameters->getABCOutputImageFormat());

    stream->writeStartElement("IMAGE");
    stream->writeTextElement(QString("FILE"), QString(m_parameters->getT1()));
    stream->writeTextElement(QString("ORIENTATION"), QString("file"));
    stream->writeEndElement();

    stream->writeStartElement("IMAGE");
    stream->writeTextElement(QString("FILE"), QString(m_parameters->getT2()));
    stream->writeTextElement(QString("ORIENTATION"), QString("file"));
    stream->writeEndElement();

    stream->writeTextElement(QString("FILTER-ITERATIONS"), QString::number(m_parameters->getNeosegParameters()->getNumberOfIterations()));//This is not used in this filter, set to 0
    stream->writeTextElement(QString("FILTER-TIME-STEP"), QString::number(m_parameters->getNeosegParameters()->getTimeStep()));
    stream->writeTextElement(QString("FILTER-METHOD"), m_parameters->getNeosegParameters()->getFilterMethod());

    stream->writeTextElement(QString("MAX-BIAS-DEGREE"), QString::number(m_parameters->getABCMaximumDegreeBiasField()));

    std::vector<double> coeffs = m_parameters->getABCPriorsCoefficients();
    for(unsigned i = 0; i < coeffs.size(); i++){
        stream->writeTextElement(QString("PRIOR"), QString::number(coeffs[i]));
    }

    stream->writeTextElement(QString("INITIAL-DISTRIBUTION-ESTIMATOR"), m_parameters->getABCInitialDistributorEstimatorType());

    stream->writeTextElement("DO-ATLAS-WARP", QString("0"));
    stream->writeTextElement("ATLAS-WARP-FLUID-ITERATIONS", QString("0"));
    stream->writeTextElement("ATLAS-WARP-FLUID-MAX-STEP", QString("0"));
    stream->writeTextElement("ATLAS-LINEAR-MAP-TYPE", QString("rigid"));
    stream->writeTextElement("IMAGE-LINEAR-MAP-TYPE", QString("id"));

}
