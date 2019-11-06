#include "../lib5grange/lib5grange.h"
#include <vector>
#include <iostream>

int main(void){
    using namespace lib5grange;    
    /* Suppose the MAC have 1024 bytes to be sent to the UE 0xCAFE
     * With the following configuration:
     * Numerology : 3   
     * Modulation : 64 QAM
     * Coderate   : 0.5
     * MIMO Scheme : Diversity
     * Number of TX antennas: 2
     */
    
    #define NUMBYTES (1024)
    char bytes_to_be_sent[NUMBYTES];        // Buffer of 1024 bytes to be transmmited

    unsigned ue_id = 0xCAFE;            // Set UE id
    unsigned numerology_id = 3;         // Set 5G Range numerology
    
    mcs_cfg_t mcs_config;               // Create MCS struct
    mcs_config.num_info_bytes = 1024;   // Set number of info BYTES to be sent
    mcs_config.modulation = QAM64;      // Set modulation
    
    float coderate = 0.75;               // Set Coderate
    
    // Set MIMO condifuration struct
    mimo_cfg_t mimo_config;
    mimo_config.scheme = DIVERSITY;
    mimo_config.num_tx_antenas = 2;
    mimo_config.precoding_mtx = 0;

    // Find the number or Resource blockes needed to the configuration proposed
    unsigned number_of_rb = lib5grange::get_num_required_rb(numerology_id, mimo_config, mcs_config.modulation, coderate, NUMBYTES * 8);
    std::cout << "The number of RB required for this allocations is " << number_of_rb << std::endl;
    
    // MAC use CSI information to to find best palce in the spectrum to allocate
    // number_of_rb contiguosly on the spectrum
    // best_rb = mac::get_best_rbs(ue_id, number_of_rb);

    allocation_cfg_t allocation_config;
    allocation_config.first_rb = 0;
    allocation_config.number_of_rb = number_of_rb;
    allocation_config.target_ue_id = ue_id;
    
    // Create a MacPDU Object to sendo to the PHY    
    lib5grange::MacPDU mac_pdu_packet;

    // popy config structs to the MacPDU object
    mac_pdu_packet.allocation_ = allocation_config;
    mac_pdu_packet.mimo_ = mimo_config;
    mac_pdu_packet.mcs_  = mcs_config; 

    // Copy the data to the Object
    mac_pdu_packet.mac_data_.resize(NUMBYTES);
    for(int i=0; i<NUMBYTES; i++){
        mac_pdu_packet.mac_data_[i] = bytes_to_be_sent[i];
    }  

    std::vector<uint8_t> bytes_to_phy_process;         //  
    mac_pdu_packet.serialize(bytes_to_phy_process);    // Serialize object into a sequence of bytes
    //macphyapi::send(bytes_to_phy_process);           // Sendo to PHY using IPC methos to be defined

}
