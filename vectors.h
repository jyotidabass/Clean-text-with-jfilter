#include <torch/script.h>

namespace torchtext {

typedef std::vector<std::string> StringList;
typedef ska_ordered::order_preserving_flat_hash_map<std::string, torch::Tensor>
    VectorsMap;
typedef ska_ordered::order_preserving_flat_hash_map<std::string, int64_t>
    IndexMap;
typedef std::tuple<
    std::string,
    std::vector<int64_t>,
    std::vector<std::string>,
    std::vector<torch::Tensor>>
    VectorsStates;

struct Vectors : torch::CustomClassHolder {
 public:
  const std::string version_str_ = "0.0.1";
  IndexMap stoi_;
  VectorsMap stovec_;
  torch::Tensor vectors_;
  torch::Tensor unk_tensor_;

  explicit Vectors(
      const IndexMap& stoi,
      torch::Tensor vectors,
      torch::Tensor unk_tensor);
  explicit Vectors(
      const std::vector<std::string>& tokens,
      const std::vector<std::int64_t>& indices,
      torch::Tensor vectors,
      torch::Tensor unk_tensor);
  std::unordered_map<std::string, int64_t> get_stoi();
  torch::Tensor __getitem__(const std::string& token);
  torch::Tensor lookup_vectors(const std::vector<std::string>& tokens);
  void __setitem__(const std::string& token, const torch::Tensor& vector);
  int64_t __len__();
};

VectorsStates _serialize_vectors(const c10::intrusive_ptr<Vectors>& self);
c10::intrusive_ptr<Vectors> _deserialize_vectors(VectorsStates states);

std::tuple<Vectors, std::vector<std::string>> _load_token_and_vectors_from_file(
    const std::string& file_path,
    const std::string& delimiter_str,
    const int64_t num_cpus,
    c10::optional<torch::Tensor> opt_unk_tensor);

} // namespace torchtext
